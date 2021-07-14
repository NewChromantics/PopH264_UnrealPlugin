// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class PopH264 : ModuleRules
{
	//	https://docs.unrealengine.com/4.26/en-US/ProductionPipelines/BuildTools/UnrealBuildTool/ThirdPartyLibraries/
	public PopH264(ReadOnlyTargetRules Target) : base(Target)
	{
		//	tell UE there's no source to compile
		Type = ModuleType.External;

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			//	this will make it static link
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "windows", "Release_x64", "PopH264.lib"));

			// Delay-load the DLL, so we can load it from the right place first
			//	gr: without this, editor will fail if it doesnt find dll at bootup
			//		but if it doesnt find DLL, we'll get an exception as we have missing symbols
			PublicDelayLoadDLLs.Add("PopH264.dll");

			// Ensure that the DLL is staged along with the executable
			var OutputPath = "$(TargetOutputDir)/PopH264.dll";
			//var SourcePath = Path.Combine(ModuleDirectory, "windows", "Release_x64", "PopH264.dll");
			var SourcePath = "$(PluginDir)/Source/ThirdParty/PopH264/windows/Release_x64/PopH264.dll";
			//RuntimeDependencies.Add("$(TargetOutputDir)/Foo.dll", Path.Combine(PluginDirectory, "Source/ThirdParty/bin/Foo.dll"));
			RuntimeDependencies.Add(OutputPath, SourcePath);

			//RuntimeDependencies.Add("$(PluginDir)/Binaries/ThirdParty/PopH264/windows/Release_x64/PopH264.dll");
			//RuntimeDependencies.Add("$(PluginDir)/Source/ThirdParty/PopH264/windows/Release_x64/PopH264.dll");
		}
		else if (Target.Platform == UnrealTargetPlatform.Mac)
		{
			//	https://www.parallelcube.com/2018/03/01/using-thirdparty-libraries-in-our-ue4-mobile-project/

			//	linked & loaded
			//	gr: NO trailing slash or linker won't find it
			PublicFrameworks.Add("$(PluginDir)/Source/ThirdParty/PopH264/PopH264.xcframework/macos-x86_64/PopH264_Osx.framework");
		}
	}
}
