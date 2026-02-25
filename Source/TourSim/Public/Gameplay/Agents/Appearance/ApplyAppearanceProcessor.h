// © 2026 Denis Sviridov. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "MassProcessor.h"
#include "AgentAppearanceFragment.h"
#include "AppearanceAppliedFragment.h"
#include "ApplyAppearanceProcessor.generated.h"


UCLASS()
class TOURSIM_API UApplyAppearanceProcessor : public UMassProcessor
{
	GENERATED_BODY()
	
public:
	UApplyAppearanceProcessor();

protected:
	virtual void ConfigureQueries() override;
	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;

private:
	FMassEntityQuery EntityQuery;
};
