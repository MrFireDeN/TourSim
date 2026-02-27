// © 2026 Denis Sviridov. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "TouristNamePoolDataAsset.h"
#include "TouristSpawnSource.h"
#include "TouristConfigSharedFragment.generated.h"

/**
 * Profile describing how to sample preferences/state at spawn time.
 * Used for spawn-source-dependent initialization.
 */
USTRUCT(BlueprintType)
struct FTouristSpawnProfile
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TourSim|Profile")
	float WBench = 0.33f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TourSim|Profile")
	float WMonument = 0.33f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TourSim|Profile")
	float WFastFood = 0.34f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TourSim|Profile", meta=(ClampMin="0.0", ClampMax="1.0"))
	float PriceSensitivity = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TourSim|Profile")
	bool bRandomizeWeights = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TourSim|Profile", meta=(ClampMin="0.0"))
	float WeightVariation = 0.20f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TourSim|Profile")
	bool bNormalizeWeights = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TourSim|Profile", meta=(ClampMin="0"))
	int32 MinMoneyCents = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TourSim|Profile", meta=(ClampMin="0"))
	int32 MaxMoneyCents = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TourSim|Profile", meta=(ClampMin="0.0"))
	float InitialNeedMin = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TourSim|Profile", meta=(ClampMin="0.0"))
	float InitialNeedMax = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TourSim|Profile", meta=(ClampMin="0.0"))
	float InitialDecisionCooldownMin = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TourSim|Profile", meta=(ClampMin="0.0"))
	float InitialDecisionCooldownMax = 1.0f;
};

/**
 * Const shared fragment that carries archetype-level configuration for tourist initialization:
 * - which name pool to use (PoolId + SoftAsset)
 * - spawn-source-dependent sampling profiles
 *
 * Stored as const shared fragment so processors can access it from an entity chunk.
 */
USTRUCT()
struct FTouristConfigSharedFragment final : public FMassConstSharedFragment
{
	GENERATED_BODY()

public:
	FName NamePoolId = NAME_None;
	TSoftObjectPtr<UTouristNamePoolDataAsset> NamePoolAsset;

	FTouristSpawnProfile DefaultProfile;
	FTouristSpawnProfile BusStopProfile;
	FTouristSpawnProfile ParkingProfile;
};
