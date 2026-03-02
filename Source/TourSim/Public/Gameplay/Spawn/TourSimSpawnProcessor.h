// © 2026 Denis Sviridov. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "MassSpawnLocationProcessor.h"
#include "TourSimSpawnProcessor.generated.h"

/**
 * SpawnDataProcessor for Tourists.
 *
 * Extends UMassSpawnLocationProcessor to additionally write spawn source type
 * to the tourist spawn source fragment, using extended AuxData:
 * FTourSimSpawnTransformData { Transforms[], SourceTypes[], bRandomize }.
 */
UCLASS()
class TOURSIM_API UTourSimSpawnProcessor final : public UMassSpawnLocationProcessor
{
	GENERATED_BODY()
	
protected:
	virtual void ConfigureQueries() override;
	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;
};
