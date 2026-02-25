// © 2026 Denis Sviridov. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "MassObserverProcessor.h"
#include "AgentAppearanceFragment.h"
#include "AgentAppearanceInitProcessor.generated.h"


class UMassEntitySubsystem;

UCLASS(BlueprintType)
class TOURSIM_API UAgentAppearanceInitProcessor : public UMassObserverProcessor
{
	GENERATED_BODY()
	
public:
	UAgentAppearanceInitProcessor();
	
protected:
	virtual void ConfigureQueries() override;
	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;

private:
	FMassEntityQuery EntityQuery;
};
