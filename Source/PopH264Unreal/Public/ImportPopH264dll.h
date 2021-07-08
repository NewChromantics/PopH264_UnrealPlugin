// Copyright (C) Condense Reality Ltd - All Rights Reserved * Unauthorized copying of this file, via any medium is strictly prohibited * Proprietary and confidential * Written by Arthur Muddiman<arthur@condensereality.com>, March 2021

#pragma once

#include "CoreMinimal.h"
#include "PopH264.h"

/**
 * 
 */

class ImportPopH264dll
{
public:
	ImportPopH264dll();
	~ImportPopH264dll();

	static int32_t	PopH264_GetVersion();
	static void		PopH264_Shutdown();
	static int32_t	PopH264_GetTestData(const char* Name, uint8_t* Buffer, int32_t BufferSize);
	static int32_t	PopH264_CreateDecoder(const char* OptionsJson, char* ErrorBuffer, int32_t ErrorBufferLength);
	static void		PopH264_DestroyDecoder(int32_t Instance);
	static void		PopH264_DestroyInstance(int32_t Instance);
	static void		PopH264_EnumDecoders(char* DecodersJsonBuffer, int32_t DecodersJsonBufferLength);
	static void		PopH264_GetMeta(int32_t Instance, int32_t* MetaValues, int32_t MetaValuesCount);
	static void		PopH264_PeekFrame(int32_t Instance, char* JsonBuffer, int32_t JsonBufferSize);
	static int32_t	PopH264_PushData(int32_t Instance, uint8_t* Data, int32_t DataSize, int32_t FrameNumber);
	static int32_t	PopH264_PushEndOfStream(int32_t Instance);
	static int32_t	PopH264_PopFrame(int32_t Instance, uint8_t* Plane0, int32_t Plane0Size, uint8_t* Plane1, int32_t Plane1Size, uint8_t* Plane2, int32_t Plane2Size);
	static void		PopH264_DecoderAddOnNewFrameCallback(int32_t Instance, PopH264_Callback* Callback, void* Meta);
	static int32_t	PopH264_CreateEncoder(const char* OptionsJson, char* ErrorBuffer, int32_t ErrorBufferSize);
	static void		PopH264_DestroyEncoder(int32_t Instance);
	static void		PopH264_EncoderPushFrame(int32_t Instance, const char* MetaJson, const uint8_t* LumaData, const uint8_t* ChromaUData, const uint8_t* ChromaVData, char* ErrorBuffer, int32_t ErrorBufferSize);
	static void		PopH264_EncoderEndOfStream(int32_t Instance);
	static int32_t	PopH264_EncoderPopData(int32_t Instance, uint8_t* DataBuffer, int32_t DataBufferSize);
	static void		PopH264_EncoderPeekData(int32_t Instance, char* MetaJsonBuffer, int32_t MetaJsonBufferSize);
	static void		PopH264_EncoderAddOnNewPacketCallback(int32_t Instance, PopH264_Callback* Callback, void* Meta);

protected:
	static bool importDLL();
	static bool importFunctions();
	static bool Import_PopH264_GetVersion();
	static bool Import_PopH264_Shutdown();
	static bool Import_PopH264_GetTestData();
	static bool Import_PopH264_CreateDecoder();
	static bool Import_PopH264_DestroyDecoder();
	static bool Import_PopH264_DestroyInstance();
	static bool Import_PopH264_EnumDecoders();
	static bool Import_PopH264_GetMeta();
	static bool Import_PopH264_PeekFrame();
	static bool Import_PopH264_PushData();
	static bool Import_PopH264_PushEndOfStream();
	static bool Import_PopH264_PopFrame();
	static bool Import_PopH264_DecoderAddOnNewFrameCallback();
	static bool Import_PopH264_CreateEncoder();
	static bool Import_PopH264_DestroyEncoder();
	static bool Import_PopH264_EncoderPushFrame();
	static bool Import_PopH264_EncoderEndOfStream();
	static bool Import_PopH264_EncoderPopData();
	static bool Import_PopH264_EncoderPeekData();
	static bool Import_PopH264_EncoderAddOnNewPacketCallback();
};
