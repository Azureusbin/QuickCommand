// Copyright © 2023 AzureuBin. All rights reserved.

using UnrealBuildTool;

public class QuickCommand : ModuleRules
{
	public QuickCommand(ReadOnlyTargetRules target) : base(target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(
			new[]
			{
				"Core",
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new []
			{
				"Projects",
				"InputCore",
				"EditorFramework",
				"UnrealEd",
				"ToolMenus",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"DeveloperSettings",
				"PropertyEditor",
				"QuickCommandCore"
			}
			);

		if (target.Version.MajorVersion == 5 && target.Version.MinorVersion == 0)
		{
			PrivateDependencyModuleNames.Add("EditorStyle");
		}
	}
}
