// © 2026 Denis Sviridov. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TouristSpawnSourceBase.generated.h"


class UBoxComponent;

/**
 * Logical spawn source type used to drive spawn-dependent initialization.
 */
UENUM(BlueprintType)
enum class ESpawnSourceType : uint8
{
	BusStop     UMETA(DisplayName="Bus Stop"),
	Parking     UMETA(DisplayName="Parking"),
	Custom      UMETA(DisplayName="Custom")
};

/**
 * Token-bucket rate limiter used to control spawn intensity.
 *
 * Accumulates tokens at a specified rate (λ).
 * Each consumed token represents permission to enqueue one agent.
 * Capacity limits burst size.
 */
USTRUCT(BlueprintType)
struct FSpawnTokenBucket
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn|Rate")
	float RatePerSec = 1.0f;	// λ

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn|Rate")
	float Capacity = 10.0f;		// max tokens

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Spawn|Rate")
	float Tokens = 0.0f;

	void Refill(const float DeltaSeconds)
	{
		Tokens = FMath::Min(Capacity, Tokens + RatePerSec * DeltaSeconds);
	}

	bool Consume(const float Amount = 1.0f)
	{
		if (Tokens >= Amount)
		{
			Tokens -= Amount;
			return true;
		}
		return false;
	}
};

/**
 * Abstract base actor representing a logical spawn source in the simulation.
 *
 * Responsible for:
 * - Maintaining local demand queue (Q(t))
 * - Applying token-bucket rate limiting (λ)
 * - Enforcing per-tick spawn limits
 * - Providing candidate spawn transforms within its geometric area
 *
 * Does NOT:
 * - Spawn entities directly
 * - Perform navigation or collision validation
 *
 * Validation and actual entity creation are handled by
 * UTourSimSpawnDataGenerator and MassSpawner pipeline.
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class TOURSIM_API ATouristSpawnSourceBase : public AActor
{
	GENERATED_BODY()

public:
	ATouristSpawnSourceBase();
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditAnywhere, Category="Spawn|Area")
	TObjectPtr<UBoxComponent> SpawnArea;
	
	UPROPERTY(EditAnywhere, Category="Spawn|Config")
	ESpawnSourceType SourceType = ESpawnSourceType::Custom;

	UPROPERTY(EditAnywhere, Category="Spawn|Config")
	int32 MaxSpawnPerTick = 5;

	UPROPERTY(EditAnywhere, Category="Spawn|Config")
	int32 MaxQueueSize = 1000;

	UPROPERTY(EditAnywhere, Category="Spawn|Rate")
	FSpawnTokenBucket TokenBucket;
	
	UPROPERTY(VisibleAnywhere, Category="Spawn|Runtime")
	float PendingQueue = 0.0f;
	
public:
	virtual void AccumulateDemand(float DeltaSeconds);
	virtual int32 ComputeSpawnQuota(int32 GlobalBudget);
	virtual bool  TryConsumeOne();
	
	virtual bool  GetCandidateTransform(FTransform& OutTransform) const;

	ESpawnSourceType GetSourceType() const { return SourceType; }
	
	float GetPendingQueue() const { return PendingQueue; }
};
