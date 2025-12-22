// © 2026 Denis Sviridov. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TourSimGameMode.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class TOURSIM_API ATourSimGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ATourSimGameMode();
	
protected:
	virtual void BeginPlay() override;
};
