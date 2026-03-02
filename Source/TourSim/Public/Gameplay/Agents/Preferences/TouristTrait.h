// © 2026 Denis Sviridov. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "MassEntityTraitBase.h"

#include "TouristPreferencesFragment.h"
#include "TouristStateFragment.h"
#include "TouristIdentityFragment.h"
#include "TouristSpawnSourceFragment.h"
#include "TouristConfigSharedFragment.h"
#include "TouristTrait.generated.h"


class UTouristNamePoolDataAsset;

/**
 * Single trait that installs all tourist fragments and attaches archetype configuration as const shared data.
 *
 * Note:
 * - Per-entity randomization and spawn-source branching is performed by an observer processor
 *   triggered by FTouristNeedsInitTag (added by the spawner after it sets SpawnSource).
 */
UCLASS(DisplayName="Tourist Trait")
class TOURSIM_API UTouristTrait final : public UMassEntityTraitBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category="TourSim|Profiles")
	FTouristSpawnProfile DefaultProfile;

	UPROPERTY(EditAnywhere, Category="TourSim|Profiles")
	FTouristSpawnProfile BusStopProfile;

	UPROPERTY(EditAnywhere, Category="TourSim|Profiles")
	FTouristSpawnProfile ParkingProfile;

protected:
	virtual void BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const override;
};
