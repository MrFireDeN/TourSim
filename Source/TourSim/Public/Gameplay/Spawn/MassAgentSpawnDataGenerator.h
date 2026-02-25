// © 2026 Denis Sviridov. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "MassEntitySpawnDataGeneratorBase.h"
#include "MassAgentSpawnDataGenerator.generated.h"

class UTourSimSpawnSubsystem;
class AAgentSpawnSourceBase;
class UMassProcessor;

/**
 * Spawn data generator producing validated spawn transforms for MassSpawner.
 *
 * Integration contract (this engine version):
 * - Returns TConstArrayView<FMassEntitySpawnDataGeneratorResult> via delegate.
 * - Each result must provide:
 *      - NumEntities
 *      - EntityConfigIndex
 *      - SpawnData (FInstancedStruct holding FMassTransformsSpawnData)
 *      - SpawnDataProcessor (typically UMassSpawnLocationProcessor)
 *
 * This generator:
 * - Queries UTourSimSpawnSubsystem for per-frame spawn requests/budget
 * - Generates up to the allowed number of transforms (never more than budget)
 * - Validates each point (NavMesh projection + ground trace + capsule overlap)
 * - Ensures Transforms.Num() == Result.NumEntities to avoid engine-side reuse filling
 */
UCLASS(Blueprintable, EditInlineNew)
class TOURSIM_API UMassAgentSpawnDataGenerator final : public UMassEntitySpawnDataGeneratorBase
{
	GENERATED_BODY()
	
public:
	virtual void Generate(
		UObject& QueryOwner, 
		TConstArrayView<FMassSpawnedEntityType> EntityTypes, 
		int32 Count, 
		FFinishedGeneratingSpawnDataSignature& FinishedGeneratingSpawnPointsDelegate) const override;
	
private:
	bool TryBuildValidatedTransform(UWorld& World, UObject& QueryOwner, AActor* SourceActor, FTransform& OutTransform) const;

	bool ValidatePoint(UWorld& World, UObject& QueryOwner, AActor* SourceActor, FTransform& InOutTransform) const;
	bool ProjectToNavmesh(UWorld& World, FVector& InOutPosition) const;
	bool GroundTrace(UWorld& World, FTransform& InOutTransform, AActor* SourceActor) const;
	bool CapsuleOverlapTest(UWorld& World, const FTransform& Transform, UObject& QueryOwner, AActor* SourceActor) const;

private:
	/** Which processor will apply SpawnData to newly spawned entities (defaults to UMassSpawnLocationProcessor). */
	UPROPERTY(EditAnywhere, Category="Spawn|Output")
	TSubclassOf<UMassProcessor> SpawnDataProcessorClass;

	/** If true, UMassSpawnLocationProcessor may randomize mapping of transforms to entities (non-deterministic runs only). */
	UPROPERTY(EditAnywhere, Category="Spawn|Output")
	bool bRandomizeOutputTransforms = false;

	UPROPERTY(EditAnywhere, Category="Spawn|Validation")
	FVector NavQueryExtent = FVector(50.f, 50.f, 200.f);

	UPROPERTY(EditAnywhere, Category="Spawn|Validation")
	float TraceHeightUp = 200.f;

	UPROPERTY(EditAnywhere, Category="Spawn|Validation")
	float TraceHeightDown = 2000.f;

	UPROPERTY(EditAnywhere, Category="Spawn|Validation")
	float CapsuleRadius = 34.f;

	UPROPERTY(EditAnywhere, Category="Spawn|Validation")
	float CapsuleHalfHeight = 88.f;

	UPROPERTY(EditAnywhere, Category="Spawn|Validation")
	float GroundZOffset = 2.f;

	UPROPERTY(EditAnywhere, Category="Spawn|Validation")
	int32 MaxPointAttemptsPerEntity = 8;
};
