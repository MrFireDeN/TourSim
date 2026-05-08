// © 2026 Denis Sviridov. MIT License.


#include "Gameplay/Agents/UI/TouristBlueprintLibrary.h"

#include "MassAgentComponent.h"
#include "MassEntitySubsystem.h"
#include "MassEntityTypes.h"
#include "Gameplay/Agents/Preferences/Fragments/TouristConfigSharedFragment.h"
#include "Gameplay/Agents/Preferences/Fragments/TouristIdentityFragment.h"
#include "Gameplay/Agents/Preferences/Subsystems/TouristNamePoolSubsystem.h"
#include "Gameplay/Agents/Preferences/Fragments/TouristPreferencesFragment.h"
#include "Gameplay/Agents/Preferences/Fragments/TouristSpawnSourceFragment.h"
#include "Gameplay/Agents/Preferences/Fragments/TouristStateFragment.h"
#include "Gameplay/Agents/UI/TouristEntityReference.h"

bool UTouristBlueprintLibrary::GetTouristSnapshot(
	UObject* WorldContextObject, 
	UTouristEntityReference* EntityRef,
	FTouristSnapshot& OutSnapshot)
{
	if (!WorldContextObject)
	{
		return false;
	}
	
	UWorld* World = WorldContextObject->GetWorld();
	if (!World)
	{
		return false;
	}
	
	UTouristNamePoolSubsystem* NamePoolSubsystem = World->GetSubsystem<UTouristNamePoolSubsystem>();
	if (!NamePoolSubsystem)
	{
		return false;
	}
	
	UMassEntitySubsystem* MassEntitySubsystem = World->GetSubsystem<UMassEntitySubsystem>();
	if (!MassEntitySubsystem)
	{
		return false;
	}
	
	FMassEntityManager& EntityManager = MassEntitySubsystem->GetMutableEntityManager();
	if (!EntityManager.IsEntityValid(EntityRef->GetHandle()))
	{
		return false;
	}
	
	return BuildSnapshot(World, EntityManager, EntityRef->GetHandle(), OutSnapshot);
}

bool UTouristBlueprintLibrary::GetTouristSnapshotFromActor(AActor* Actor, FTouristSnapshot& OutSnapshot)
{
	if (!Actor)
	{
		return false;
	}
	
	UWorld* World = Actor->GetWorld();
	if (!World)
	{
		return false;
	}
	
	UMassAgentComponent* MassAgentComponent = Actor->GetComponentByClass<UMassAgentComponent>();
	if (!MassAgentComponent)
	{
		return false;
	}
	
	const FMassEntityHandle Entity = MassAgentComponent->GetEntityHandle();

	UMassEntitySubsystem* MassSubsystem = World->GetSubsystem<UMassEntitySubsystem>();
	if (!MassSubsystem)
	{
		return false;
	}
	
	FMassEntityManager& EntityManager = MassSubsystem->GetMutableEntityManager();
	
	return BuildSnapshot(World, EntityManager, Entity, OutSnapshot);
}

bool UTouristBlueprintLibrary::BuildSnapshot(
	UWorld* World,
	FMassEntityManager& EntityManager, 
	const FMassEntityHandle Entity,
	FTouristSnapshot& OutSnapshot)
{
	const FTouristPreferencesFragment* Prefs = EntityManager.GetFragmentDataPtr<FTouristPreferencesFragment>(Entity);
	const FTouristStateFragment* State = EntityManager.GetFragmentDataPtr<FTouristStateFragment>(Entity);
	const FTouristIdentityFragment* Identity = EntityManager.GetFragmentDataPtr<FTouristIdentityFragment>(Entity);
	const FTouristSpawnSourceFragment* Source = EntityManager.GetFragmentDataPtr<FTouristSpawnSourceFragment>(Entity);
	
	if (!Prefs || !State || !Identity || !Source)
	{
		return false;
	}
	
	OutSnapshot.ShortId = Identity->ShortId;
	OutSnapshot.MoneyCents = State->MoneyCents;
	
	OutSnapshot.RestNeed = State->RestNeed;
	OutSnapshot.CultureNeed = State->CultureNeed;
	OutSnapshot.HungerNeed = State->HungerNeed;
	
	OutSnapshot.WBench = Prefs->WBench;
	OutSnapshot.WMonument = Prefs->WMonument;
	OutSnapshot.WFastFood = Prefs->WFastFood;
	OutSnapshot.PriceSensitivity = Prefs->PriceSensitivity;
	
	OutSnapshot.SpawnSource = static_cast<uint8>(Source->Source);
	
	FString BaseName;

	UTouristNamePoolSubsystem* NameSubsystem = World->GetSubsystem<UTouristNamePoolSubsystem>();
	
	UE_LOG(LogTemp, Error, TEXT("Name Subsystem = %s"), NameSubsystem ? *NameSubsystem->GetName() : TEXT("False"));
	
	if (NameSubsystem && NameSubsystem->TryGetName(Identity->NameIndex, BaseName))
	{
		OutSnapshot.DisplayName = BaseName + TEXT(" #") + FString::Printf(TEXT("%06X"), Identity->ShortId);
	}
	else
	{
		// Fallback if name pool missing or index invalid
		OutSnapshot.DisplayName = TEXT("Tourist #") + FString::Printf(TEXT("%06X"), Identity->ShortId);
	}
	
	return true;
}
