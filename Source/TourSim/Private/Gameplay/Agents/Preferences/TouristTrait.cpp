// © 2026 Denis Sviridov. MIT License.


#include "Gameplay/Agents/Preferences/TouristTrait.h"

#include "MassEntityTemplateRegistry.h"
#include "Gameplay/Agents/Preferences/TouristPreferencesFragment.h"
#include "Gameplay/Agents/Preferences/TouristConfigSharedFragment.h"

void UTouristTrait::BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const
{
	BuildContext.AddFragment<FTouristSpawnSourceFragment>();
	BuildContext.AddFragment<FTouristPreferencesFragment>();
	BuildContext.AddFragment<FTouristStateFragment>();
	BuildContext.AddFragment<FTouristIdentityFragment>();
}
