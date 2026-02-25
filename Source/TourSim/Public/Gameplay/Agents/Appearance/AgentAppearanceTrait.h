// © 2026 Denis Sviridov. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "MassEntityTraitBase.h"
#include "AgentAppearanceFragment.h"
#include "AgentAppearanceTrait.generated.h"


UCLASS()
class TOURSIM_API UAgentAppearanceTrait : public UMassEntityTraitBase
{
	GENERATED_BODY()
	
public:
	virtual void BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const override;
};
