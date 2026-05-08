// © 2026 Denis Sviridov. MIT License.


#include "Gameplay/Agents/Preferences/Processors/UTouristNeedGrowthProcessor.h"

#include "MassAIBehaviorTypes.h"
#include "MassExecutionContext.h"
#include "Gameplay/Agents/Preferences/Fragments/TouristPreferencesFragment.h"
#include "Gameplay/Agents/Preferences/Fragments/TouristStateFragment.h"

UUTouristNeedGrowthProcessor::UUTouristNeedGrowthProcessor()
{
	ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::Behavior;
	ExecutionFlags = static_cast<int32>(EProcessorExecutionFlags::All);
}

void UUTouristNeedGrowthProcessor::ConfigureQueries()
{
	EntityQuery.AddRequirement<FTouristStateFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddRequirement<FTouristPreferencesFragment>(EMassFragmentAccess::ReadOnly);
	EntityQuery.RegisterWithProcessor(*this);
}

void UUTouristNeedGrowthProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	const float CurrentTime = Context.GetWorld()->TimeSeconds;
	
	if (CurrentTime < NextNeedUpdateTime)
		return;
	
	NextNeedUpdateTime = CurrentTime + 0.5f;
	
	const float DeltaTime = Context.GetDeltaTimeSeconds();
	
	EntityQuery.ForEachEntityChunk(EntityManager, Context, [this, DeltaTime](FMassExecutionContext& ChunkContext)
	{
		const TArrayView<FTouristStateFragment> States = 
			ChunkContext.GetMutableFragmentView<FTouristStateFragment>();
		
		const TConstArrayView<FTouristPreferencesFragment> Preferences = 
			ChunkContext.GetFragmentView<FTouristPreferencesFragment>();
		
		const int32 NumEntities = ChunkContext.GetNumEntities();
		
		for (int32 i = 0; i < NumEntities; ++i)
		{
			FTouristStateFragment& State = States[i];
			const FTouristPreferencesFragment& Prefs = Preferences[i];
			
			State.RestNeed += BaseNeedIncreasePerSecond * Prefs.WBench * DeltaTime;
			State.CultureNeed += BaseNeedIncreasePerSecond * Prefs.WMonument * DeltaTime;
			State.HungerNeed += BaseNeedIncreasePerSecond * Prefs.WFastFood * DeltaTime;
			
			State.Sanitize();
		}
	});
}
