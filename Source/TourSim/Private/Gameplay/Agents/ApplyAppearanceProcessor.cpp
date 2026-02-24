// © 2026 Denis Sviridov. MIT License.


#include "Gameplay/Agents/ApplyAppearanceProcessor.h"

#include "MassActorSubsystem.h"
#include "Gameplay/Agents/AgentAppearanceFragment.h"
#include "MassRepresentationFragments.h"

static uint32 HashAppearance(const FAgentAppearanceFragment& A)
{
	// Достаточно для MVP. Если надо — сделаем стабильнее/точнее.
	uint32 H = 0;
	H = HashCombineFast(H, GetTypeHash(A.TopColor));
	H = HashCombineFast(H, GetTypeHash(A.BottomColor));
	H = HashCombineFast(H, GetTypeHash(A.HairColor));
	return H;
}

static void ApplyToActor(AActor* Actor, const FAgentAppearanceFragment& Appearance)
{
	USkeletalMeshComponent* Mesh = Actor->FindComponentByClass<USkeletalMeshComponent>();
	if (!Mesh)
	{
		UE_LOG(LogTemp, Error, TEXT("ApplyToActor: No SkeletalMeshComponent"));
		return;
	}

	// 0 = Body, 1 = Hair
	if (UMaterialInstanceDynamic* BodyDMI = Mesh->CreateDynamicMaterialInstance(0))
	{
		BodyDMI->SetVectorParameterValue(TEXT("TopColor"), Appearance.TopColor);
		BodyDMI->SetVectorParameterValue(TEXT("BottomColor"), Appearance.BottomColor);
	}

	if (UMaterialInstanceDynamic* HairDMI = Mesh->CreateDynamicMaterialInstance(1))
	{
		HairDMI->SetVectorParameterValue(TEXT("HairColor"), Appearance.HairColor);
	}
}

UApplyAppearanceProcessor::UApplyAppearanceProcessor()
{
	ExecutionFlags = (int32)EProcessorExecutionFlags::All;
	ProcessingPhase = EMassProcessingPhase::PostPhysics;
	
	bRequiresGameThreadExecution = true;
}

void UApplyAppearanceProcessor::ConfigureQueries()
{
	EntityQuery.AddRequirement<FAgentAppearanceFragment>(EMassFragmentAccess::ReadOnly);
	EntityQuery.AddRequirement<FMassRepresentationFragment>(EMassFragmentAccess::ReadOnly);
	EntityQuery.AddRequirement<FMassActorFragment>(EMassFragmentAccess::ReadOnly);
	EntityQuery.AddRequirement<FAppearanceAppliedFragment>(EMassFragmentAccess::ReadWrite);

	EntityQuery.RegisterWithProcessor(*this);
}

void UApplyAppearanceProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	check(IsInGameThread());
	
	EntityQuery.ForEachEntityChunk(EntityManager, Context, [&](FMassExecutionContext& Ctx)
	{
		const auto Appearances = Ctx.GetFragmentView<FAgentAppearanceFragment>();
		const auto Reps        = Ctx.GetFragmentView<FMassRepresentationFragment>();
		auto Applied           = Ctx.GetMutableFragmentView<FAppearanceAppliedFragment>();

		auto Actors = Ctx.GetFragmentView<FMassActorFragment>();
		
		for (int32 i = 0; i < Ctx.GetNumEntities(); ++i)
		{
			const FMassRepresentationFragment& Rep = Reps[i];

			const bool bIsSpawnedActor =
				(Rep.CurrentRepresentation == EMassRepresentationType::HighResSpawnedActor) ||
				(Rep.CurrentRepresentation == EMassRepresentationType::LowResSpawnedActor);

			if (!bIsSpawnedActor)
				continue;

			AActor* Actor = const_cast<AActor*>(Actors[i].Get());
			if (!Actor)
				continue;

			const FObjectKey ActorKey(Actor);
			const uint32 ColorHash = HashAppearance(Appearances[i]);

			// Если actor тот же и цвета те же — ничего не делаем
			if (Applied[i].LastActorKey == ActorKey && Applied[i].LastColorHash == ColorHash)
				continue;

			ApplyToActor(Actor, Appearances[i]);

			Applied[i].LastActorKey = ActorKey;
			Applied[i].LastColorHash = ColorHash;
		}
	});
}
