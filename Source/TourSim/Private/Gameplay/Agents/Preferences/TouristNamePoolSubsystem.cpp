// © 2026 Denis Sviridov. MIT License.


#include "Gameplay/Agents/Preferences/TouristNamePoolSubsystem.h"

bool UTouristNamePoolSubsystem::TryGetName(int32 NameIndex, FString& OutName) const
{
	const int32 Num = Names.Num();
	if (Num <= 0)
	{
		return false;
	}

	const int32 SafeIndex = (NameIndex >= 0) ? (NameIndex % Num) : (FMath::Abs(NameIndex) % Num);
	OutName = Names[SafeIndex];
	return true;
}
