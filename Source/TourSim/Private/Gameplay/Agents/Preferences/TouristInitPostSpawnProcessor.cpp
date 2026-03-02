// © 2026 Denis Sviridov. MIT License.


#include "Gameplay/Agents/Preferences/TouristInitPostSpawnProcessor.h"

#include "MassExecutionContext.h"
#include "Gameplay/Agents/Preferences/TouristConfigSharedFragment.h"
#include "Gameplay/Agents/Preferences/TouristIdentityFragment.h"
#include "Gameplay/Agents/Preferences/TouristNamePoolSubsystem.h"
#include "Gameplay/Agents/Preferences/TouristPreferencesFragment.h"
#include "Gameplay/Agents/Preferences/TouristSpawnSourceFragment.h"
#include "Gameplay/Agents/Preferences/TouristStateFragment.h"

namespace TourSim
{
	static uint32 MakeStableSeed(const FMassEntityHandle Entity)
	{
		return GetTypeHash(Entity);
	}

	static int32 MakeShortId(uint32 Seed)
	{
		return static_cast<int32>(Seed & 0x00FFFFFFu);
	}

	template<typename T>
	static T RandRangeClamped(FRandomStream& Rng, T MinV, T MaxV)
	{
		if constexpr (std::is_floating_point_v<T>)
		{
			return (MaxV > MinV) ? Rng.FRandRange(MinV, MaxV) : MinV;
		}
		else
		{
			return (MaxV > MinV) ? Rng.RandRange(MinV, MaxV) : MinV;
		}
	}
}

UTouristInitPostSpawnProcessor::UTouristInitPostSpawnProcessor()
{
	EntityQuery.RegisterWithProcessor(*this); 
	
	bAutoRegisterWithProcessingPhases = false;
	ExecutionFlags = (int32)EProcessorExecutionFlags::All;
}

void UTouristInitPostSpawnProcessor::ConfigureQueries()
{
	EntityQuery.AddRequirement<FTouristSpawnSourceFragment>(EMassFragmentAccess::ReadOnly);
	EntityQuery.AddRequirement<FTouristPreferencesFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddRequirement<FTouristStateFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddRequirement<FTouristIdentityFragment>(EMassFragmentAccess::ReadWrite);

	EntityQuery.AddConstSharedRequirement<FTouristConfigSharedFragment>(EMassFragmentPresence::All);

}

void UTouristInitPostSpawnProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	UWorld* World = Context.GetWorld();
	if (!World)
	{
		return;
	}
	
	UTouristNamePoolSubsystem* NameSubsystem = World->GetSubsystem<UTouristNamePoolSubsystem>();
	
	EntityQuery.ForEachEntityChunk(EntityManager, Context, [World, NameSubsystem](FMassExecutionContext& ExecCtx)
	{
		const FTouristConfigSharedFragment& Config = 
			ExecCtx.GetConstSharedFragment<const FTouristConfigSharedFragment>();
		
		const int32 Num = ExecCtx.GetNumEntities();
		
		auto Sources = ExecCtx.GetFragmentView<FTouristSpawnSourceFragment>();
		auto Prefs = ExecCtx.GetMutableFragmentView<FTouristPreferencesFragment>();
		auto State = ExecCtx.GetMutableFragmentView<FTouristStateFragment>();
		auto Id = ExecCtx.GetMutableFragmentView<FTouristIdentityFragment>();
		
		for (int32 i = 0; i < Num; ++i)
		{
			const FMassEntityHandle Entity = ExecCtx.GetEntity(i);
			
			const uint32 Seed = TourSim::MakeStableSeed(Entity);
			FRandomStream Rng((int32)Seed);
			
			// --- 1) Select profile by spawn source
			
			const ESpawnSourceType SourceType = Sources[i].Source;
			
			const FTouristSpawnProfile* Profile = &Config.DefaultProfile;
			
			switch (SourceType)
			{
			case ESpawnSourceType::BusStop:
				Profile = &Config.BusStopProfile;
				break;
			case ESpawnSourceType::Parking:
				Profile = &Config.ParkingProfile;
				break;
			default:
				break;
			}
			
			// --- 2) Preferences (profile defines bounds only)
			
			Prefs[i].WBench = 
				TourSim::RandRangeClamped(Rng, Profile->WBenchMin, Profile->WBenchMax);
			
			Prefs[i].WMonument = 
				TourSim::RandRangeClamped(Rng, Profile->WMonumentMin, Profile->WMonumentMax);
			
			Prefs[i].WFastFood = 
				TourSim::RandRangeClamped(Rng, Profile->WFastFoodMin, Profile->WFastFoodMax);
			
			Prefs[i].PriceSensitivity = 
				TourSim::RandRangeClamped(Rng, Profile->PriceSensitivityMin, Profile->PriceSensitivityMax);
			
			Prefs[i].Sanitize();
			
			// --- 3) Runtime state
			
			State[i].MoneyCents		= TourSim::RandRangeClamped(Rng, Profile->MinMoneyCents, Profile->MaxMoneyCents);
			State[i].RestNeed		= TourSim::RandRangeClamped(Rng, Profile->InitialNeedMin, Profile->InitialNeedMax);
			State[i].CultureNeed	= TourSim::RandRangeClamped(Rng, Profile->InitialNeedMin, Profile->InitialNeedMax);
			State[i].HungerNeed		= TourSim::RandRangeClamped(Rng, Profile->InitialNeedMin, Profile->InitialNeedMax);
			
			State[i].NextDecisionTimeSeconds = 
				World->GetTimeSeconds() + 
				TourSim::RandRangeClamped(Rng, 
					Profile->InitialDecisionCooldownMin, 
					Profile->InitialDecisionCooldownMax);
			
			State[i].Sanitize();
			
			// --- 4) Identity
			
			Id[i].ShortId = TourSim::MakeShortId(Seed);
			
			if (NameSubsystem && !Config.NamePoolId.IsNone())
			{
				const int32 PoolSize = NameSubsystem->GetPoolSize(Config.NamePoolId);
				Id[i].NameIndex = (PoolSize > 0) ? static_cast<int32>(Seed % PoolSize) : -1;
			}
			else
			{
				Id[i].NameIndex = -1;
			}
		}
	});
}
