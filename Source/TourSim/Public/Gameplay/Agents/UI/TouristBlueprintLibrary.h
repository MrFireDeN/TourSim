// © 2026 Denis Sviridov. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "TouristSnapshot.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TouristBlueprintLibrary.generated.h"

struct FMassEntityManager;
struct FMassEntityHandle;
class UTouristEntityReference;

/**
 * Blueprint access layer for tourist data queries.
 */
UCLASS()
class TOURSIM_API UTouristBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category="Tourist")
	static bool GetTouristSnapshot(
		UObject* WorldContextObject, 
		UTouristEntityReference* EntityRef,
		FTouristSnapshot& OutSnapshot);
	
	UFUNCTION(BlueprintCallable, Category="Tourist")
	static bool GetTouristSnapshotFromActor(AActor* Actor, FTouristSnapshot& OutSnapshot);
	
	static bool BuildSnapshot(
		UWorld* World,
		FMassEntityManager& EntityManager,
		FMassEntityHandle Entity,
		FTouristSnapshot& OutSnapshot);
};
