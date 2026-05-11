// © 2026 Denis Sviridov. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TouristBase.generated.h"

class UArrowComponent;
class USkeletalMeshComponent;

/**
 * Minimal Pawn representation for Mass Framework.
 * Inherits from APawn for Animation Blueprint compatibility (TryGetPawnOwner).
 * All heavy/unnecessary features are disabled in the constructor.
 */
UCLASS(Blueprintable, BlueprintType, meta = (DisplayName = "Tourist Base"))
class TOURSIM_API ATouristBase : public APawn
{
	GENERATED_BODY()

public:
	ATouristBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	/** Returns the skeletal mesh component. */
	USkeletalMeshComponent* GetMesh() const { return Mesh; }
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual FVector GetVelocity() const override { return CurrentVelocity; }
	
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Mesh;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Animation")
	float RotationSpeed = 5.0f;
	
private:
	FVector CurrentVelocity = FVector::ZeroVector;
	FVector LastLocation = FVector::ZeroVector;
	bool bFirstTick = true;
};
