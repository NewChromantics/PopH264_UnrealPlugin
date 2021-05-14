// Copyright Epic Games, Inc. All Rights Reserved.

#include "PopH264Unreal.h"
#include "Core.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"

//	gr: add include path to plugin build.cs
//#include "PopH264UnrealLibrary/ExampleLibrary.h"
#include "PopH264UnrealLibrary/com.newchromantics.poph264-1.3.38/windows/Release_x64/PopH264.h"

#define LOCTEXT_NAMESPACE "FPopH264UnrealModule"

void FPopH264UnrealModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	// Get the base directory of this plugin
	FString BaseDir = IPluginManager::Get().FindPlugin("PopH264Unreal")->GetBaseDir();

	//	gr: don't need to manually load the framework, (symbols are lazy-loaded on-access)
	//		we can still dload() the framework for completeness
	//		but we will need to explicitly load handle for the DLL
/*
	// Add on the relative location of the third party dll and load it
	FString LibraryPath;
#if PLATFORM_WINDOWS
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/PopH264UnrealLibrary/Win64/ExampleLibrary.dll"));
#elif PLATFORM_MAC
    LibraryPath = FPaths::Combine(*BaseDir, TEXT("Source/ThirdParty/PopH264UnrealLibrary/Mac/Release/libExampleLibrary.dylib"));
#endif // PLATFORM_WINDOWS

	ExampleLibraryHandle = !LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;
*/
	if ( true )//ExampleLibraryHandle)
	{
		// Call the test function in the third party library that opens a message box
		//ExampleLibraryFunction();
		auto Version = PopH264_GetVersion();
		FString VersionString = FString::Printf( TEXT( "PopH264 version %d" ), Version );
		UE_LOG( LogTemp, Warning, TEXT( "Your string: %s" ), *VersionString );
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("ThirdPartyLibraryError", "Failed to load example third party library"));
	}
	
}

void FPopH264UnrealModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	// Free the dll handle
	FPlatformProcess::FreeDllHandle(ExampleLibraryHandle);
	ExampleLibraryHandle = nullptr;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FPopH264UnrealModule, PopH264Unreal)
