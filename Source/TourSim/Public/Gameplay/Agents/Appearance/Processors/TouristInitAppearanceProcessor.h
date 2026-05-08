// © 2026 Denis Sviridov. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "MassObserverProcessor.h"
#include "TouristInitAppearanceProcessor.generated.h"


class UMassEntitySubsystem;

UCLASS(BlueprintType)
class TOURSIM_API UTouristInitAppearanceProcessor : public UMassObserverProcessor
{
	GENERATED_BODY()
	
public:
	UTouristInitAppearanceProcessor();
	
protected:
	virtual void ConfigureQueries() override;
	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;

private:
	FMassEntityQuery EntityQuery;
};
