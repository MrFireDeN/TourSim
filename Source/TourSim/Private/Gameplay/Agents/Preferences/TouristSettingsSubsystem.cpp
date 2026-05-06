// © 2026 Denis Sviridov. MIT License.


#include "Gameplay/Agents/Preferences/TouristSettingsSubsystem.h"

void UTouristSettingsSubsystem::SetCurrentConfig(const FTouristConfigSharedFragment& NewConfig)
{
	CurrentConfig = NewConfig;
	bConfigSet = true;
}

void UTouristSettingsSubsystem::ResetToDefault()
{
	CurrentConfig = FTouristConfigSharedFragment();
	bConfigSet = false;
}
