// © 2026 Denis Sviridov. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "PoITypes.generated.h"

struct FMassEntityHandle;


UENUM(BlueprintType, meta = (DisplayName = "Point of Interest Type"))
enum class EPoiType : uint8
{
	Cultural       UMETA(DisplayName = "Cultural Attraction"),
	Entertainment  UMETA(DisplayName = "Entertainment"),
	Restaurant     UMETA(DisplayName = "Restaurant"),
	Accommodation  UMETA(DisplayName = "Accommodation"),
	Transportation UMETA(DisplayName = "Transportation Hub"),
	Shopping       UMETA(DisplayName = "Shopping"),
	Natural        UMETA(DisplayName = "Natural Site"),
	Custom         UMETA(DisplayName = "Custom")
};

USTRUCT(BlueprintType)
struct FWorkingHours
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Working Hours")
	int32 OpenHour = 9; // 9:00
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Working Hours")
	int32 CloseHour = 18; // 18:00
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Working Hours")
	bool bIsAllDay = false;

	bool IsOpenAtTime(const FDateTime& DateTime) const
	{
		const int32 CurrentHour = DateTime.GetHour();
		
		return bIsAllDay || (CurrentHour >= OpenHour && CurrentHour <= CloseHour);
	}
};

USTRUCT(BlueprintType)
struct FPoiConfiguration
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGuid PoiId = FGuid::NewGuid();
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DisplayName;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPoiType Type = EPoiType::Cultural;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector WorldLocation = FVector::ZeroVector;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D GeoCoordinates;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0))
	int32 MaxCapacity = 50;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0.0))
	float BaseAttractiveness = 1.0f;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWorkingHours OperatingHours;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, float> MassBehaviorModifiers;
};

USTRUCT(BlueprintType)
struct FPoiRuntimeState
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<AActor>> CurrentVisitors;
    
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FDateTime LastVisitTime;
    
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CurrentCrowdDensity = 0.0f;
    
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsOpen = true;
};