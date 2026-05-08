// © 2026 Denis Sviridov. MIT License.


#include "Gameplay/Spawn/TourSimSpawnProcessor.h"

#include "MassCommonFragments.h"
#include "MassCommonUtils.h"
#include "MassEntityManager.h"
#include "MassExecutionContext.h"

#include "Gameplay/Agents/Preferences/Fragments/TouristSpawnSourceFragment.h"
#include "Gameplay/Spawn/TourSimSpawnData.h"

void UTourSimSpawnProcessor::ConfigureQueries()
{
	Super::ConfigureQueries();
	
	EntityQuery.AddRequirement<FTouristSpawnSourceFragment>(EMassFragmentAccess::ReadWrite);
}

void UTourSimSpawnProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	if (!ensure(Context.ValidateAuxDataType<FTourSimSpawnData>()))
	{
		UE_VLOG_UELOG(this, LogMass, Log,
			TEXT("Invalid AuxData type. Expected FTourSimSpawnTransformData. Spawn init skipped."));
		return;
	}

	const UWorld* World = EntityManager.GetWorld();
	check(World);

	FTourSimSpawnData& AuxData = Context.GetMutableAuxData().GetMutable<FTourSimSpawnData>();
	TArray<FTransform>& Transforms = AuxData.Transforms;
	TArray<ESpawnSourceType>& SourceTypes = AuxData.SourceTypes;

	const int32 NumSpawnEntries = Transforms.Num();
	if (NumSpawnEntries == 0)
	{
		UE_VLOG_UELOG(this, LogMass, Error, TEXT("No spawn transforms provided. Spawn init skipped."));
		return;
	}

	if (SourceTypes.Num() != NumSpawnEntries)
	{
		UE_VLOG_UELOG(this, LogMass, Error,
			TEXT("Spawn aux data mismatch: Transforms=%d SourceTypes=%d. Spawn init skipped."),
			NumSpawnEntries, SourceTypes.Num());
		return;
	}
	
	int32 NumRequiredEntries = 0;
	EntityQuery.ForEachEntityChunk(EntityManager, Context, [&NumRequiredEntries](const FMassExecutionContext& Ctx)
	{
		NumRequiredEntries += Ctx.GetNumEntities();
	});
	
	const int32 NumToAdd = NumRequiredEntries - NumSpawnEntries;
	if (NumToAdd > 0)
	{
		UE_VLOG_UELOG(this, LogMass, Warning,
			TEXT("Not enough spawn entries (%d) for entities (%d). Reusing existing entries to fill %d missing."),
			NumSpawnEntries, NumRequiredEntries, NumToAdd);

		Transforms.AddUninitialized(NumToAdd);
		SourceTypes.AddUninitialized(NumToAdd);

		for (int32 i = 0; i < NumToAdd; ++i)
		{
			const int32 Pick = RandomStream.RandRange(0, NumSpawnEntries - 1);
			Transforms[NumSpawnEntries + i] = Transforms[Pick];
			SourceTypes[NumSpawnEntries + i] = SourceTypes[Pick];
		}
	}

	// Randomized assignment (engine-style)
	if (AuxData.bRandomize && !UE::Mass::Utils::IsDeterministic())
	{
		EntityQuery.ForEachEntityChunk(EntityManager, Context, [this, &Transforms, &SourceTypes](FMassExecutionContext& ChunkCtx)
		{
			const int32 NumEntities = ChunkCtx.GetNumEntities();

			TArrayView<FTransformFragment> Xforms = ChunkCtx.GetMutableFragmentView<FTransformFragment>();
			TArrayView<FTouristSpawnSourceFragment> Sources = ChunkCtx.GetMutableFragmentView<FTouristSpawnSourceFragment>();

			for (int32 i = 0; i < NumEntities; ++i)
			{
				const int32 AuxIndex = RandomStream.RandRange(0, Transforms.Num() - 1);

				Xforms[i].GetMutableTransform() = Transforms[AuxIndex];
				Sources[i].Source = SourceTypes[AuxIndex];

				Transforms.RemoveAtSwap(AuxIndex, EAllowShrinking::No);
				SourceTypes.RemoveAtSwap(AuxIndex, EAllowShrinking::No);
			}
		});
	}
	else
	{
		int32 NextIndex = 0;

		EntityQuery.ForEachEntityChunk(EntityManager, Context, [&Transforms, &SourceTypes, &NextIndex](FMassExecutionContext& ChunkCtx)
		{
			const int32 NumEntities = ChunkCtx.GetNumEntities();

			TArrayView<FTransformFragment> Xforms = ChunkCtx.GetMutableFragmentView<FTransformFragment>();
			TArrayView<FTouristSpawnSourceFragment> Sources = ChunkCtx.GetMutableFragmentView<FTouristSpawnSourceFragment>();

			check(NextIndex + NumEntities <= Transforms.Num());

			FMemory::Memcpy(Xforms.GetData(), Transforms.GetData() + NextIndex, sizeof(FTransform) * NumEntities);

			for (int32 i = 0; i < NumEntities; ++i)
			{
				Sources[i].Source = SourceTypes[NextIndex + i];
			}

			NextIndex += NumEntities;
		});
	}
}
