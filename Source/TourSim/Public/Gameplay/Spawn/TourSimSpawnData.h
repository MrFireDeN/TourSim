// © 2026 Denis Sviridov. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "TourSimSpawnData.generated.h"

/**
 * Spawn data payload for MassSpawner.
 *
 * Contains one transform per entity that should be spawned for a single EntityType slice.
 * This payload is embedded into FMassEntitySpawnDataGeneratorResult::SpawnData via FInstancedStruct.
 */
USTRUCT()
struct FTourSimSpawnTransformData
{
	GENERATED_BODY()

	/** Per-entity spawn transforms. Size must be equal to FMassEntitySpawnDataGeneratorResult::NumEntities. */
	UPROPERTY()
	TArray<FTransform> Transforms;
};
