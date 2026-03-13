// © 2026 Denis Sviridov. MIT License.


#include "Gameplay/PoI/PointOfInterest.h"
#include "Components/BillboardComponent.h"
#include "Components/SphereComponent.h"
#include "Gameplay/PoI/PoiRecord.h"


APointOfInterest::APointOfInterest()
{
	PrimaryActorTick.bCanEverTick = false;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);
	
	InteractionVolume = CreateDefaultSubobject<USphereComponent>("InteractionVolume");
	InteractionVolume->SetupAttachment(RootComponent);
	
	InteractionVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	InteractionVolume->SetGenerateOverlapEvents(false);
	
#if WITH_EDITORONLY_DATA
	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	Billboard->SetupAttachment(RootComponent);
#endif
}

FPoiRecord APointOfInterest::BuildPoiRecord(const int32 InId) const
{
	FPoiRecord Record;
	Record.Id = InId;
	Record.Type = Type;
	Record.Location = GetActorLocation();
	Record.InteractionRadius = FMath::Max(0.0f, InteractionVolume->GetScaledSphereRadius());
	Record.PriceCents = FMath::Max(0, PriceCents);
	Record.Capacity = FMath::Max(1, Capacity);
	Record.TouristCount = FMath::Clamp(TouristCount, 0, Record.Capacity);
	Record.bEnabled = bEnabled;
	return Record;
}

bool APointOfInterest::IsAccessible() const
{
	return bEnabled && TouristCount < Capacity;
}

void APointOfInterest::SetTouristCount(const int32 InTouristCount)
{
	TouristCount = FMath::Clamp(InTouristCount, 0, Capacity);
}
