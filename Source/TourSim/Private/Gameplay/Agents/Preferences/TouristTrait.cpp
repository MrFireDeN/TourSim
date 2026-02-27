// © 2026 Denis Sviridov. MIT License.


#include "Gameplay/Agents/Preferences/TouristTrait.h"

#include "MassEntityTemplateRegistry.h"
#include "Gameplay/Agents/Preferences/TouristPreferencesFragment.h"
#include "Gameplay/Agents/Preferences/TouristConfigSharedFragment.h"

void UTouristTrait::BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const
{
	BuildContext.AddFragment<FAgentSpawnSourceFragment>();
	BuildContext.AddFragment<FTouristPreferencesFragment>();
	BuildContext.AddFragment<FTouristStateFragment>();
	BuildContext.AddFragment<FTouristIdentityFragment>();

	FTouristConfigSharedFragment Config;
	Config.NamePoolId      = NamePoolId;
	Config.NamePoolAsset   = NamePoolAsset;
	Config.DefaultProfile  = DefaultProfile;
	Config.BusStopProfile  = BusStopProfile;
	Config.ParkingProfile  = ParkingProfile;

	const FConstSharedStruct SharedConfig = FConstSharedStruct::Make(Config);
	BuildContext.AddConstSharedFragment(SharedConfig);
}
