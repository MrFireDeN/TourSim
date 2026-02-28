// © 2026 Denis Sviridov. MIT License.


#include "Gameplay/Agents/Appearance/TouristAppearanceTrait.h"
#include "MassEntityTemplateRegistry.h"
#include "Gameplay/Agents/Appearance/TouristAppearanceAppliedFragment.h"

void UTouristAppearanceTrait::BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const
{
	BuildContext.AddFragment<FTouristAppearanceFragment>();
	BuildContext.AddFragment<FTouristAppearanceAppliedFragment>();
}
