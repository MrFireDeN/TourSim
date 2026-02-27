// © 2026 Denis Sviridov. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TouristNamePoolDataAsset.generated.h"

/**
 * Data asset containing a list of names used for tourist identity.
 * Stored in Content to allow easy editing without recompilation.
 */
UCLASS(BlueprintType)
class TOURSIM_API UTouristNamePoolDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="TourSim|Names")
	TArray<FString> Names;
	
	virtual void PostLoad() override;
};
