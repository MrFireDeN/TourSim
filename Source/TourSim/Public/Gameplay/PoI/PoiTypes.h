// © 2026 Denis Sviridov. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "PoITypes.generated.h"

struct FMassEntityHandle;

/**
 * Domain type of a point of interest used by tourists during intent evaluation.
 */
UENUM(BlueprintType, meta = (DisplayName = "Point of Interest Type"))
enum class EPoiType : uint8
{
	None		UMETA(DisplayName="None"),
	Bench		UMETA(DisplayName="Bench"),
	Monument	UMETA(DisplayName="Monument"),
	FastFood	UMETA(DisplayName="FastFood")
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
