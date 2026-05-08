// © 2026 Denis Sviridov. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "../Fragments/TouristConfigSharedFragment.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TouristSettingsSubsystem.generated.h"

/**
 * GameInstance-scoped storage for runtime tourist configuration.
 * Set from main menu UI before traveling to park level.
 * Read by UTouristInitPostSpawnProcessor during entity initialization.
 */
UCLASS()
class TOURSIM_API UTouristSettingsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	/**
	 * Assign active configuration.
	 * @param NewConfig full config containing Default, BusStop and Parking profiles.
	 */
	UFUNCTION(BlueprintCallable, Category = "TourSim|Settings")
	void SetCurrentConfig(const FTouristConfigSharedFragment& NewConfig);
	
	/**
	 * Retrieve current configuration.
	 * @return const reference to stored config, default‑constructed if never set.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TourSim|Settings")
	const FTouristConfigSharedFragment& GetCurrentConfig() const { return CurrentConfig; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TourSim|Settings")
	bool IsConfigSet() const { return bConfigSet; }
	
	UFUNCTION(BlueprintCallable, Category = "TourSim|Settings")
	void ResetToDefault();
	
private:
	UPROPERTY()
	FTouristConfigSharedFragment CurrentConfig;
	
	UPROPERTY()
	bool bConfigSet = false;
};
