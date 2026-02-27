// © 2026 Denis Sviridov. MIT License.


#include "Gameplay/Agents/Preferences/TouristNamePoolDataAsset.h"

void UTouristNamePoolDataAsset::PostLoad()
{
	Super::PostLoad();

	if (Names.Num() == 0)
	{
		Names = {
			TEXT("Alex"),
			TEXT("Sam"),
			TEXT("Taylor"),
			TEXT("Jordan"),
			TEXT("Casey"),
			TEXT("Morgan"),
			TEXT("Riley"),
			TEXT("Avery"),
			TEXT("Quinn"),
			TEXT("Skyler"),
			TEXT("Rowan"),
			TEXT("Cameron"),
			TEXT("Reese"),
			TEXT("Emerson"),
			TEXT("Finley"),
			TEXT("Dakota"),
			TEXT("Hayden"),
			TEXT("Jamie"),
			TEXT("Kendall"),
			TEXT("Phoenix"),
			TEXT("Sage"),
			TEXT("River"),
			TEXT("Arden"),
			TEXT("Blake"),
			TEXT("Charlie"),
			TEXT("Devin"),
			TEXT("Elliot"),
			TEXT("Harper"),
			TEXT("Jules"),
			TEXT("Kai"),
			TEXT("Logan"),
			TEXT("Micah"),
			TEXT("Noel"),
			TEXT("Parker"),
			TEXT("Robin"),
			TEXT("Shawn"),
			TEXT("Toby"),
			TEXT("Zion")
		};
	}
}
