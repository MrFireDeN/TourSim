// © 2026 Denis Sviridov. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "MassProcessor.h"
#include "TouristApplyAppearanceProcessor.generated.h"


UCLASS()
class TOURSIM_API UTouristApplyAppearanceProcessor : public UMassProcessor
{
	GENERATED_BODY()
	
public:
	UTouristApplyAppearanceProcessor();

protected:
	virtual void ConfigureQueries() override;
	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;

private:
	FMassEntityQuery EntityQuery;
	FMassEntityQuery ISMQuery;
};
