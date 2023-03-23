using UnrealBuildTool;

public class QuickCommandCore : ModuleRules
{
    public QuickCommandCore(ReadOnlyTargetRules InTarget) : base(InTarget)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Projects"
            }
        );
    }
}