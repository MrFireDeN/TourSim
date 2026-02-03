// © 2026 Denis Sviridov. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PointOfInterest.generated.h"

struct FMassEntityHandle;
struct FPoiConfiguration;
class UMassAgentSubsystem;
class UBoxComponent;
class UWidgetComponent;
class UPoiDataComponent;

UCLASS(Abstract, Blueprintable, BlueprintType)
class TOURSIM_API APointOfInterest : public AActor
{
	GENERATED_BODY()

public:
	APointOfInterest();

protected:
	
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UPoiDataComponent> DataComponent;
    
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Mesh;
    
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UWidgetComponent> InfoWidgetComponent;
    
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBoxComponent> InteractionVolume;
    
	// MassAI Integration
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MassAI")
	TObjectPtr<UMassAgentSubsystem> MassAgentSubsystem;
    
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MassAI")
	//FMassEntityConfigHandle AgentConfigHandle;

public:
	UFUNCTION(BlueprintCallable, Category = "Point of Interest")
	virtual void InitializeFromData(const FPoiConfiguration& Config);
    
	UFUNCTION()
	void OnMassAgentArrived(FMassEntityHandle AgentHandle);
    
	UFUNCTION()
	void OnMassAgentDeparted(FMassEntityHandle AgentHandle);
};
