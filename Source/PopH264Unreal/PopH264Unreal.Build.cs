// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PopH264Unreal : ModuleRules
{
	public PopH264Unreal(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		/*gr: doesnt work
		var SourcePath = System.IO.Path.Combine(ModuleDirectory, "..", "ThirdParty", "PopH264", "windows", "Release_x64", "PopH264.dll");
		Tools.DotNETCommon.Log.TraceInformation("PopH264Unreal path");
		Tools.DotNETCommon.Log.TraceInformation(SourcePath);
		RuntimeDependencies.Add(SourcePath);
		RuntimeDependencies.Add("$(PluginDir)/Source/ThirdParty/PopH264/windows/Release_x64/PopH264.dll");
		*/

		PublicIncludePaths.AddRange(
			new string[] {
				//"PopH264"
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				//"PopH264"
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"PopH264",
				"Projects",
				"Json"
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				// ... add private dependencies that you statically link with here ...	
				"CoreUObject",
				"Engine"
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
