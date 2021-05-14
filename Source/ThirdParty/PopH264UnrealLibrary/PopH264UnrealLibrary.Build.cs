// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class PopH264UnrealLibrary : ModuleRules
{
	public PopH264UnrealLibrary(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			// Add the import library
			
			
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "com.newchromantics.poph264-1.3.38", "windows", "Release_x64", "PopH264.lib"));

			// Delay-load the DLL, so we can load it from the right place first
			PublicDelayLoadDLLs.Add("PopH264.dll");

			// Ensure that the DLL is staged along with the executable
			RuntimeDependencies.Add("$(PluginDir)/Binaries/ThirdParty/PopH264UnrealLibrary/com.newchromantics.poph264-1.3.38/windows/Release_x64/PopH264.dll");
		}
		else if (Target.Platform == UnrealTargetPlatform.Mac)
		{
			//	https://www.parallelcube.com/2018/03/01/using-thirdparty-libraries-in-our-ue4-mobile-project/
			//PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "FMOD", "Includes"));

			//	linked & loaded
			//	gr: NO trailing slash or linker won't find it
			PublicFrameworks.Add("$(PluginDir)/Source/ThirdParty/PopH264UnrealLibrary/com.newchromantics.poph264-1.3.38/PopH264.xcframework/macos-x86_64/PopH264_Osx.framework");
		}
	}
}
