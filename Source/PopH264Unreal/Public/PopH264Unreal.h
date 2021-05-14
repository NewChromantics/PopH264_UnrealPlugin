// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FPopH264DecoderInstance;


class FPopH264UnrealModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	/** Handle to the test dll we will load */
	void*	ExampleLibraryHandle = nullptr;
};


class FPopH264DecoderInstance
{
public:
	FPopH264DecoderInstance();
	~FPopH264DecoderInstance();

	//	push data
	//	read new texture
	//	get other meta

private:
	uint32_t	mInstanceHandle = 0;	//	allocated by dll
};
