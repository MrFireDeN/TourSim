// © 2026 Denis Sviridov. MIT License.


#include "Gameplay/Agents/Preferences/TouristIdentityTrait.h"

#include "MassEntityTemplateRegistry.h"
#include "Gameplay/Agents/Preferences/Fragments/TouristPreferencesFragment.h"
#include "Gameplay/Agents/Preferences/Fragments/TouristConfigSharedFragment.h"

void UTouristIdentityTrait::BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const
{
	BuildContext.AddFragment<FTouristSpawnSourceFragment>();
	BuildContext.AddFragment<FTouristPreferencesFragment>();
	BuildContext.AddFragment<FTouristStateFragment>();
	BuildContext.AddFragment<FTouristIdentityFragment>();
}
