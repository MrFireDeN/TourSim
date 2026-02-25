// © 2026 Denis Sviridov. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "TourSimSpawnSubsystem.generated.h"

class AAgentSpawnSourceBase;

/**
 * Represents a planned spawn allocation for a specific source.
 *
 * Produced by UTourSimSpawnSubsystem and consumed by
 * UMassAgentSpawnDataGenerator during spawn data generation.
 */
USTRUCT(BlueprintType)
struct FSpawnRequest
{
	GENERATED_BODY()

	UPROPERTY()
	TWeakObjectPtr<AAgentSpawnSourceBase> Source;

	UPROPERTY()
	int32 Count = 0;
};

/**
 * World-level subsystem managing all active spawn sources.
 *
 * Responsibilities:
 * - Registering and tracking AAgentSpawnSourceBase instances
 * - Updating demand accumulation each frame
 * - Enforcing global spawn budget per tick
 * - Producing a fair distribution of FSpawnRequest objects
 *
 * This subsystem does not generate spawn locations itself.
 * It only determines how many entities should be spawned
 * and from which source during the current frame.
 */
UCLASS()
class TOURSIM_API UTourSimSpawnSubsystem final : public UTickableWorldSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UTourSimSpawnSubsystem, STATGROUP_Tickables); }
	virtual bool IsTickable() const override { return true; }
	
	void RegisterSource(AAgentSpawnSourceBase* Source);
	void UnregisterSource(AAgentSpawnSourceBase* Source);

	void BuildSpawnRequests(TArray<FSpawnRequest>& OutRequests);
	
private:
	void CompactSources();

private:
	UPROPERTY(EditAnywhere, Category="Spawn|Budget")
	int32 GlobalMaxSpawnPerTick = 50;

	TArray<TWeakObjectPtr<AAgentSpawnSourceBase>> Sources;
	TArray<FSpawnRequest> CachedRequests;

	int32 RoundRobinIndex = 0;
};
