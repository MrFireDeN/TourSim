// © 2026 Denis Sviridov. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "TouristNamePoolSubsystem.generated.h"

class UTouristNamePoolDataAsset;
/**
 * World subsystem that owns and serves name pools for UI and spawn-time identity assignment.
 *
 * Design:
 * - Assets are loaded synchronously on first use
 * - Fragments store only NameIndex + ShortId; actual strings are resolved here.
 */
UCLASS()
class TOURSIM_API UTouristNamePoolSubsystem final : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category="TourSim|Names")
	bool TryGetName(int32 NameIndex, FString& OutName) const;
	
private:
	UPROPERTY()
	TArray<FString> Names = {
		TEXT("Alex"),
		TEXT("Sam"),
		TEXT("Taylor"),
		TEXT("Jordan"),
		TEXT("Casey"),
		TEXT("Morgan"),
		TEXT("Riley"),
		TEXT("Avery"),
		TEXT("Quinn"),
		TEXT("Skyler"),
		TEXT("Rowan"),
		TEXT("Cameron"),
		TEXT("Reese"),
		TEXT("Emerson"),
		TEXT("Finley"),
		TEXT("Dakota"),
		TEXT("Hayden"),
		TEXT("Jamie"),
		TEXT("Kendall"),
		TEXT("Phoenix"),
		TEXT("Sage"),
		TEXT("River"),
		TEXT("Arden"),
		TEXT("Blake"),
		TEXT("Charlie"),
		TEXT("Devin"),
		TEXT("Elliot"),
		TEXT("Harper"),
		TEXT("Jules"),
		TEXT("Kai"),
		TEXT("Logan"),
		TEXT("Micah"),
		TEXT("Noel"),
		TEXT("Parker"),
		TEXT("Robin"),
		TEXT("Shawn"),
		TEXT("Toby"),
		TEXT("Zion")
	};
	
public:
	FORCEINLINE int32 GetNamesNum() const { return Names.Num(); }
};
