// © 2026 Denis Sviridov. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "Gameplay/Spawn/TouristSpawnSourceBase.h"
#include "TouristSpawnSourceFragment.generated.h"

/**
 * Fragment that marks which spawn source produced this agent.
 * Must be set by spawning logic before init-tag is added.
 */
USTRUCT(BlueprintType)
struct FTouristSpawnSourceFragment final : public FMassFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TourSim|Spawn")
	ESpawnSourceType Source = ESpawnSourceType::Custom;
};
