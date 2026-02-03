// © 2026 Denis Sviridov. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "PoiTypes.h"
#include "Components/ActorComponent.h"
#include "PoiDataComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TOURSIM_API UPoiDataComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPoiDataComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Poi|Data", meta = (ShowOnlyInnerProperties))
	FPoiConfiguration Configuration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Poi|State")
	FPoiRuntimeState RuntimeState;
    
	UFUNCTION(BlueprintCallable, Category = "Poi|Data")
	void LoadFromGeoJson(const FString& JsonString);
    
	UFUNCTION(BlueprintCallable, Category = "Poi|Data")
	FString ExportToGeoJson() const;
    
	UFUNCTION(BlueprintCallable, Category = "Poi|MassAI")
	float CalculateAttractivenessScore() const;
    
	UFUNCTION(BlueprintCallable, Category = "Poi|MassAI")
	bool CanAcceptMoreAgents() const;
};
