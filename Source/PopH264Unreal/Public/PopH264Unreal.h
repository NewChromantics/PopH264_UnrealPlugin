// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"
#include "Templates/UniquePtr.h"

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


class PopH264FrameMeta_t
{
public:
	size_t	FrameNumber = 0;
};

class FPopH264DecoderInstance
{
public:
	//	gr: this is mostly just a helper for implementation reference
	static TUniquePtr<FPopH264DecoderInstance>	AllocDecoder();

public:
	FPopH264DecoderInstance();
	~FPopH264DecoderInstance();

	void					PushH264Data(const TArray<uint8_t>& H264Data,size_t FrameNumber);
	bool					PushTestData(const char* TestDataName,size_t FrameNumber);	//	returns false if invalid test data, or failed to push


	//	gr: must be called on game thread
	UTexture2D*		PopFrame(PopH264FrameMeta_t& Meta);

private:
	uint32_t		mInstanceHandle = 0;	//	allocated by dll
};
