// © 2026 Denis Sviridov. MIT License.


#include "Gameplay/PoI/PoiDataComponent.h"


UPoiDataComponent::UPoiDataComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPoiDataComponent::LoadFromGeoJson(const FString& JsonString)
{
}

FString UPoiDataComponent::ExportToGeoJson() const
{
	FString JsonString;
	return JsonString;
}

float UPoiDataComponent::CalculateAttractivenessScore() const
{
	return 0.0f;
}

bool UPoiDataComponent::CanAcceptMoreAgents() const
{
	return true;
}

