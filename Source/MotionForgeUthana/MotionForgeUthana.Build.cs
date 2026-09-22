using UnrealBuildTool;

public class MotionForgeUthana : ModuleRules
{
	public MotionForgeUthana(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"MotionForge",  // the capability this provider plugs into
				"HTTP",         // the public header hands out IHttpRequest pointers
			}
			);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Json",
				"DeveloperSettings", // the plan and price page
				"AssetRegistry",     // counting uploaded characters without loading them
				"Slate",             // opening the Keys page from a setup step
				"SlateCore",
				"Settings",          // opening this plugin's own settings page
			}
			);
	}
}
