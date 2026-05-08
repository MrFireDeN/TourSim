// © 2026 Denis Sviridov. MIT License.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Agents/Preferences/Fragments/TouristConfigSharedFragment.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TouristConfigJSONUtils.generated.h"

/**
 * Utility library for saving and loading FTouristConfigSharedFragment to/from JSON files.
 * Provides file dialog integration for user-friendly file selection.
 * All functions are callable from Blueprints.
 */
UCLASS(Blueprintable)
class TOURSIM_API UTouristConfigJSONUtils final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/**
	* Opens a "Save As" file dialog, then saves the given tourist configuration to the selected JSON file.
	* The user chooses the file path interactively; the function handles serialization and file writing.
	*
	* @param Config The configuration structure to save (contains default profile and spawn-source profiles).
	* @return True if the file was saved successfully, false otherwise (e.g., user cancelled or write error).
	*/
	UFUNCTION(BlueprintCallable, Category="TourSim|Config")
	static bool SaveTouristConfigToFile(const FTouristConfigSharedFragment& Config);
	
	/**
	 * Opens an "Open File" dialog, loads a tourist configuration from the selected JSON file,
	 * and populates the output structure with the loaded data.
	 *
	 * @param OutConfig The configuration structure that will receive the loaded data.
	 * @return True if the file was loaded and parsed successfully, false otherwise (e.g., user cancelled, file not found, or invalid JSON).
	 */
	UFUNCTION(BlueprintCallable, Category="Tourist|Config")
	static bool LoadTouristConfigFromFile(FTouristConfigSharedFragment& OutConfig);
};
