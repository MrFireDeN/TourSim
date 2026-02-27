// © 2026 Denis Sviridov. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "TouristNeedsInitTag.generated.h"

/**
 * Tag used to trigger spawn-time initialization of tourist parameters.
 * Spawner should add this tag AFTER setting FAgentSpawnSourceFragment.Source.
 */
USTRUCT()
struct FTouristNeedsInitTag final : public FMassTag
{
	GENERATED_BODY()
};
