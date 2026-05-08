// © 2026 Denis Sviridov. MIT License.


#include "Gameplay/Agents/Appearance/TouristAppearanceTrait.h"
#include "MassEntityTemplateRegistry.h"
#include "Gameplay/Agents/Appearance/Fragments/TouristAppearanceAppliedFragment.h"
#include "Gameplay/Agents/Appearance/Fragments/TouristAppearanceFragment.h"

void UTouristAppearanceTrait::BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const
{
	BuildContext.AddFragment<FTouristAppearanceFragment>();
	BuildContext.AddFragment<FTouristAppearanceAppliedFragment>();
}
