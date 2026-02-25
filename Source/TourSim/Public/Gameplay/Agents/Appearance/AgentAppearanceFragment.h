// © 2026 Denis Sviridov. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "MassEntityTypes.h"
#include "AgentAppearanceFragment.generated.h"


USTRUCT(BlueprintType)
struct FAgentAppearanceFragment : public FMassFragment
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor TopColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor BottomColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor HairColor = FLinearColor::Black;
};
