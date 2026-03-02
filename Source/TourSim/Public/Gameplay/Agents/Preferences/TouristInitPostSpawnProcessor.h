// © 2026 Denis Sviridov. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "MassProcessor.h"
#include "TouristInitPostSpawnProcessor.generated.h"

/**
 * Post-spawn processor that initializes tourist runtime parameters.
 *
 * Reads:
 *  - FTouristSpawnSourceFragment
 *  - FTouristConfigSharedFragment (const shared)
 *
 * Writes:
 *  - FTouristPreferencesFragment
 *  - FTouristStateFragment
 *  - FTouristIdentityFragment
 *
 * Profile defines only randomization bounds.
 */
UCLASS()
class TOURSIM_API UTouristInitPostSpawnProcessor : public UMassProcessor
{
	GENERATED_BODY()
	
public:
	UTouristInitPostSpawnProcessor();
	
protected:
	virtual void ConfigureQueries() override;
	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;
	
private:
	FMassEntityQuery EntityQuery;
};
