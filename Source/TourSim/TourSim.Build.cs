// © 2026 Denis Sviridov. MIT License.

using UnrealBuildTool;

public class TourSim : ModuleRules
{
	public TourSim(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			
			// UI
			"UMG",
			
			// Mass
			"MassEntity",
			"MassCommon",
			"MassCrowd",        
			"MassActors",       
			"MassMovement",   
			"MassSpawner",      
			"MassLOD",          
			"MassRepresentation",
			"MassNavigation",
			"MassAIBehavior",
			"MassSignals",
			
			// Nav
			"NavigationSystem",
			"StateTreeModule",
			
			// Utils
			"StructUtils",
			"Json",
			"JsonUtilities",
			"DesktopPlatform",
			"Slate",
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
