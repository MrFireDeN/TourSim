// © 2026 Denis Sviridov. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TourSimPawn.generated.h"

UCLASS(Blueprintable, BlueprintType)
class TOURSIM_API ATourSimPawn : public APawn
{
	GENERATED_BODY()

public:
	ATourSimPawn();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
