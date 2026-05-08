// © 2026 Denis Sviridov. MIT License.

#include "Gameplay/Agents/Appearance/Processors/TouristInitAppearanceProcessor.h"

#include "MassCommonFragments.h"
#include "MassEntityQuery.h"
#include "MassExecutionContext.h"
#include "Gameplay/Agents/Appearance/Fragments/TouristAppearanceFragment.h"

UTouristInitAppearanceProcessor::UTouristInitAppearanceProcessor()
{
	ObservedType = FTouristAppearanceFragment::StaticStruct();
	Operation = EMassObservedOperation::Add;
	
	ExecutionFlags = (int32)EProcessorExecutionFlags::All;
	
	ProcessingPhase = EMassProcessingPhase::PrePhysics;
}

void UTouristInitAppearanceProcessor::ConfigureQueries()
{
	EntityQuery.AddRequirement<FTouristAppearanceFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.RegisterWithProcessor(*this);
}


static inline FLinearColor RandomColor(FRandomStream& Stream)
{
	return FLinearColor(Stream.FRand(), Stream.FRand(), Stream.FRand(), 1.f);
}

void UTouristInitAppearanceProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	UE_LOG(LogTemp, Warning, TEXT("Appearance init running")); 
	
	EntityQuery.ForEachEntityChunk(EntityManager, Context, [&](FMassExecutionContext& Ctx)
	{
		auto Appearances = Ctx.GetMutableFragmentView<FTouristAppearanceFragment>();

		for (int32 i = 0; i < Ctx.GetNumEntities(); ++i)
		{
			const uint32 Seed = HashCombineFast((uint32)Ctx.GetEntity(i).Index, 0x9E3779B9u);
			FRandomStream Stream(Seed);

			Appearances[i].TopColor = RandomColor(Stream);
			Appearances[i].BottomColor = RandomColor(Stream);
			Appearances[i].HairColor = RandomColor(Stream);
		}
	});
}
