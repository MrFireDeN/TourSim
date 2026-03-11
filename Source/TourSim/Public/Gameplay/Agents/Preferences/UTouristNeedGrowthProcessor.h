// © 2026 Denis Sviridov. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "MassProcessor.h"
#include "UTouristNeedGrowthProcessor.generated.h"

/**
 * Mass processor that gradually increases tourist needs over time.
 * Growth speed depends on the normalized preference weights of each tourist.
 */
UCLASS()
class TOURSIM_API UUTouristNeedGrowthProcessor : public UMassProcessor
{
	GENERATED_BODY()
	
public:
	UUTouristNeedGrowthProcessor();
	
protected:
	/** Base growth rate applied per second before preference scaling. */
	UPROPERTY(EditAnywhere, Category="TourSim|Needs", meta=(ClampMin="0.01"))
	float BaseNeedIncreasePerSecond = 0.01f;
	
	virtual void ConfigureQueries() override;
	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;
	
private:
	FMassEntityQuery EntityQuery;
};
