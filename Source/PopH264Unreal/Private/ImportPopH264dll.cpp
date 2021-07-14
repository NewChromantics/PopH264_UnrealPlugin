// Copyright (C) Condense Reality Ltd - All Rights Reserved * Unauthorized copying of this file, via any medium is strictly prohibited * Proprietary and confidential * Written by Arthur Muddiman<arthur@condensereality.com>, March 2021


#include "ImportPopH264dll.h"

typedef int32_t(*_PopH264_GetVersion)();
typedef void(*_PopH264_Shutdown)();
typedef int32_t(*_PopH264_GetTestData)(const char* Name, uint8_t* Buffer, int32_t BufferSize);
typedef int32_t(*_PopH264_CreateDecoder)(const char* OptionsJson, char* ErrorBuffer, int32_t ErrorBufferLength);
typedef void(*_PopH264_DestroyDecoder)(int32_t Instance);
typedef void(*_PopH264_DestroyInstance)(int32_t Instance);
typedef void(*_PopH264_EnumDecoders)(char* DecodersJsonBuffer, int32_t DecodersJsonBufferLength);
typedef void(*_PopH264_GetMeta)(int32_t Instance, int32_t* MetaValues, int32_t MetaValuesCount);
typedef void(*_PopH264_PeekFrame)(int32_t Instance, char* JsonBuffer, int32_t JsonBufferSize);
typedef int32_t(*_PopH264_PushData)(int32_t Instance, uint8_t* Data, int32_t DataSize, int32_t FrameNumber);
typedef int32_t(*_PopH264_PushEndOfStream)(int32_t Instance);
typedef int32_t(*_PopH264_PopFrame)(int32_t Instance, uint8_t* Plane0, int32_t Plane0Size, uint8_t* Plane1, int32_t Plane1Size, uint8_t* Plane2, int32_t Plane2Size);
typedef void(*_PopH264_DecoderAddOnNewFrameCallback)(int32_t Instance, PopH264_Callback* Callback, void* Meta);
typedef int32_t(*_PopH264_CreateEncoder)(const char* OptionsJson, char* ErrorBuffer, int32_t ErrorBufferSize);
typedef void(*_PopH264_DestroyEncoder)(int32_t Instance);
typedef void(*_PopH264_EncoderPushFrame)(int32_t Instance, const char* MetaJson, const uint8_t* LumaData, const uint8_t* ChromaUData, const uint8_t* ChromaVData, char* ErrorBuffer, int32_t ErrorBufferSize);
typedef void(*_PopH264_EncoderEndOfStream)(int32_t Instance);
typedef int32_t(*_PopH264_EncoderPopData)(int32_t Instance, uint8_t* DataBuffer, int32_t DataBufferSize);
typedef void(*_PopH264_EncoderPeekData)(int32_t Instance, char* MetaJsonBuffer, int32_t MetaJsonBufferSize);
typedef void(*_PopH264_EncoderAddOnNewPacketCallback)(int32_t Instance, PopH264_Callback* Callback, void* Meta);

_PopH264_GetVersion m_getPopH264VersionFromDLL;
_PopH264_Shutdown m_PopH264_ShutdownFromDLL;
_PopH264_GetTestData m_PopH264_GetTestDataFromDLL;
_PopH264_CreateDecoder m_PopH264_CreateDecoderFromDLL;
_PopH264_DestroyDecoder m_PopH264_DestroyDecoderFromDLL;
_PopH264_DestroyInstance m_PopH264_DestroyInstanceFromDLL;
_PopH264_EnumDecoders m_PopH264_EnumDecodersFromDLL;
_PopH264_GetMeta m_PopH264_GetMetaFromDLL;
_PopH264_PeekFrame m_PopH264_PeekFrameFromDLL;
_PopH264_PushData m_PopH264_PushDataFromDLL;
_PopH264_PushEndOfStream m_PopH264_PushEndOfStreamFromDLL;
_PopH264_PopFrame m_PopH264_PopFrameFromDLL;
_PopH264_DecoderAddOnNewFrameCallback m_PopH264_DecoderAddOnNewFrameCallbackFromDLL;
_PopH264_CreateEncoder m_PopH264_CreateEncoderFromDLL;
_PopH264_DestroyEncoder m_PopH264_DestroyEncoderFromDLL;
_PopH264_EncoderPushFrame m_PopH264_EncoderPushFrameFromDLL;
_PopH264_EncoderEndOfStream m_PopH264_EncoderEndOfStreamFromDLL;
_PopH264_EncoderPopData m_PopH264_EncoderPopDataFromDLL;
_PopH264_EncoderPeekData m_PopH264_EncoderPeekDataFromDLL;
_PopH264_EncoderAddOnNewPacketCallback m_PopH264_EncoderAddOnNewPacketCallbackFromDLL;

void* v_dllHandle;

ImportPopH264dll::ImportPopH264dll()
{
	if (importDLL())
	{
		UE_LOG(LogTemp, Warning, TEXT("Imported PopH264.dll"));
		if (importFunctions())
		{
			UE_LOG(LogTemp, Warning, TEXT("Functions Imported..."));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Could not Import All functions"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Error Loading PopH264.dll"));
	}
}

ImportPopH264dll::~ImportPopH264dll()
{
}


bool ImportPopH264dll::importDLL()
{
	//TODO: Platforms other than Win64
	FString filePath = *FPaths::ProjectPluginsDir();
	filePath += ("PopH264_UnrealPlugin/Source/ThirdParty/PopH264UnrealLibrary/com.newchromantics.poph264-1.3.38/windows/Release_x64/PopH264.dll");

	if (FPaths::FileExists(filePath))
	{
		v_dllHandle = FPlatformProcess::GetDllHandle(*filePath);
		if (v_dllHandle != NULL)
		{
			return true;
		}
	}
	return false;
}

bool ImportPopH264dll::importFunctions()
{
	if (!Import_PopH264_GetVersion())
	{
		UE_LOG(LogTemp, Error, TEXT("Could not import Get Version Function"));
		return false;
	}

	if (!Import_PopH264_Shutdown())
	{
		UE_LOG(LogTemp, Error, TEXT("Could not import Shutdown Function"));
		return false;
	}

	if (!Import_PopH264_GetTestData())
	{
		UE_LOG(LogTemp, Error, TEXT("Could not import Get Test Data Function"));
		return false;
	}

	if (!Import_PopH264_CreateDecoder())
	{
		UE_LOG(LogTemp, Error, TEXT("Could not import Create Decoder Function"));
		return false;
	}

	if (!Import_PopH264_DestroyDecoder())
	{
		UE_LOG(LogTemp, Error, TEXT("Could not import Destroy Decoder Function"));
		return false;
	}

	if (!Import_PopH264_DestroyInstance())
	{
		UE_LOG(LogTemp, Error, TEXT("Could not import Destroy Instance Function"));
		return false;
	}

	if (!Import_PopH264_EnumDecoders())
	{
		UE_LOG(LogTemp, Error, TEXT("Could not import EnumDecoders Function"));
		return false;
	}

	if (!Import_PopH264_GetMeta())
	{
		UE_LOG(LogTemp, Error, TEXT("Could not import GetMeta Function"));
		return false;
	}

	if (!Import_PopH264_PeekFrame())
	{
		UE_LOG(LogTemp, Error, TEXT("Could not import PeekFrame Function"));
		return false;
	}

	if (!Import_PopH264_PushData())
	{
		UE_LOG(LogTemp, Error, TEXT("Could not import PushData Function"));
		return false;
	}

	if (!Import_PopH264_PushEndOfStream())
	{
		UE_LOG(LogTemp, Error, TEXT("Could not import PushEndOfStream Function"));
		return false;
	}

	if (!Import_PopH264_PopFrame())
	{
		UE_LOG(LogTemp, Error, TEXT("Could not import PopFrame Function"));
		return false;
	}

	if (!Import_PopH264_DecoderAddOnNewFrameCallback())
	{
		UE_LOG(LogTemp, Error, TEXT("Could not import DecoderAddOnNewFrameCallback Function"));
		return false;
	}

	if (!Import_PopH264_CreateEncoder())
	{
		UE_LOG(LogTemp, Error, TEXT("Could not import CreateEncoder Function"));
		return false;
	}

	if (!Import_PopH264_DestroyEncoder())
	{
		UE_LOG(LogTemp, Error, TEXT("Could not import DestroyEncoder Function"));
		return false;
	}

	if (!Import_PopH264_EncoderPushFrame())
	{
		UE_LOG(LogTemp, Error, TEXT("Could not import EncoderPushFrame Function"));
		return false;
	}

	if (!Import_PopH264_EncoderEndOfStream())
	{
		UE_LOG(LogTemp, Error, TEXT("Could not import EncoderEndOfStream Function"));
		return false;
	}

	if (!Import_PopH264_EncoderPopData())
	{
		UE_LOG(LogTemp, Error, TEXT("Could not import EncoderPopData Function"));
		return false;
	}

	if (!Import_PopH264_EncoderPeekData())
	{
		UE_LOG(LogTemp, Error, TEXT("Could not import EncoderPeekData Function"));
		return false;
	}

	if (!Import_PopH264_EncoderAddOnNewPacketCallback())
	{
		UE_LOG(LogTemp, Error, TEXT("Could not import EncoderAddOnNewPacketCallback Function"));
		return false;
	}

	return true;
}


bool ImportPopH264dll::Import_PopH264_GetVersion()
{
	if (v_dllHandle != NULL)
	{
		m_getPopH264VersionFromDLL = NULL;
		FString procName = "PopH264_GetVersion";
		m_getPopH264VersionFromDLL = (_PopH264_GetVersion)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
		if (m_getPopH264VersionFromDLL != NULL)
		{
			return true;
		}
	}

	return false;
}

bool ImportPopH264dll::Import_PopH264_Shutdown()
{
	if (v_dllHandle != NULL)
	{
		m_PopH264_ShutdownFromDLL = NULL;
		FString procName = "PopH264_Shutdown";
		m_PopH264_ShutdownFromDLL = (_PopH264_Shutdown)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
		if (m_PopH264_ShutdownFromDLL != NULL)
		{
			return true;
		}
	}

	return false;
}

bool ImportPopH264dll::Import_PopH264_GetTestData()
{
	if (v_dllHandle != NULL)
	{
		m_PopH264_GetTestDataFromDLL = NULL;
		FString procName = "PopH264_GetTestData";
		m_PopH264_GetTestDataFromDLL = (_PopH264_GetTestData)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
		if (m_PopH264_GetTestDataFromDLL != NULL)
		{
			return true;
		}
	}

	return false;
}

bool ImportPopH264dll::Import_PopH264_CreateDecoder()
{
	if (v_dllHandle != NULL)
	{
		m_PopH264_CreateDecoderFromDLL = NULL;
		FString procName = "PopH264_CreateDecoder";
		m_PopH264_CreateDecoderFromDLL = (_PopH264_CreateDecoder)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
		if (m_PopH264_CreateDecoderFromDLL != NULL)
		{
			return true;
		}
	}

	return false;
}

bool ImportPopH264dll::Import_PopH264_DestroyDecoder()
{
	if (v_dllHandle != NULL)
	{
		m_PopH264_DestroyDecoderFromDLL = NULL;
		FString procName = "PopH264_DestroyDecoder";
		m_PopH264_DestroyDecoderFromDLL = (_PopH264_DestroyDecoder)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
		if (m_PopH264_DestroyDecoderFromDLL != NULL)
		{
			return true;
		}
	}

	return false;
}

bool ImportPopH264dll::Import_PopH264_DestroyInstance()
{
	if (v_dllHandle != NULL)
	{
		m_PopH264_DestroyInstanceFromDLL = NULL;
		FString procName = "PopH264_DestroyInstance";
		m_PopH264_DestroyInstanceFromDLL = (_PopH264_DestroyInstance)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
		if (m_PopH264_DestroyInstanceFromDLL != NULL)
		{
			return true;
		}
	}

	return false;
}

bool ImportPopH264dll::Import_PopH264_EnumDecoders()
{
	if (v_dllHandle != NULL)
	{
		m_PopH264_EnumDecodersFromDLL = NULL;
		FString procName = "PopH264_EnumDecoders";
		m_PopH264_EnumDecodersFromDLL = (_PopH264_EnumDecoders)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
		if (m_PopH264_EnumDecodersFromDLL != NULL)
		{
			return true;
		}
	}

	return false;
}

bool ImportPopH264dll::Import_PopH264_GetMeta()
{
	if (v_dllHandle != NULL)
	{
		m_PopH264_GetMetaFromDLL = NULL;
		FString procName = "PopH264_GetMeta";
		m_PopH264_GetMetaFromDLL = (_PopH264_GetMeta)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
		if (m_PopH264_GetMetaFromDLL != NULL)
		{
			return true;
		}
	}

	return false;
}

bool ImportPopH264dll::Import_PopH264_PeekFrame()
{
	if (v_dllHandle != NULL)
	{
		m_PopH264_PeekFrameFromDLL = NULL;
		FString procName = "PopH264_PeekFrame";
		m_PopH264_PeekFrameFromDLL = (_PopH264_PeekFrame)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
		if (m_PopH264_PeekFrameFromDLL != NULL)
		{
			return true;
		}
	}

	return false;
}

bool ImportPopH264dll::Import_PopH264_PushData()
{
	if (v_dllHandle != NULL)
	{
		m_PopH264_PushDataFromDLL = NULL;
		FString procName = "PopH264_PushData";
		m_PopH264_PushDataFromDLL = (_PopH264_PushData)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
		if (m_PopH264_PushDataFromDLL != NULL)
		{
			return true;
		}
	}

	return false;
}

bool ImportPopH264dll::Import_PopH264_PushEndOfStream()
{
	if (v_dllHandle != NULL)
	{
		m_PopH264_PushEndOfStreamFromDLL = NULL;
		FString procName = "PopH264_PushEndOfStream";
		m_PopH264_PushEndOfStreamFromDLL = (_PopH264_PushEndOfStream)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
		if (m_PopH264_PushEndOfStreamFromDLL != NULL)
		{
			return true;
		}
	}

	return false;
}

bool ImportPopH264dll::Import_PopH264_PopFrame()
{
	if (v_dllHandle != NULL)
	{
		m_PopH264_PopFrameFromDLL = NULL;
		FString procName = "PopH264_PopFrame";
		m_PopH264_PopFrameFromDLL = (_PopH264_PopFrame)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
		if (m_PopH264_PopFrameFromDLL != NULL)
		{
			return true;
		}
	}

	return false;
}

bool ImportPopH264dll::Import_PopH264_DecoderAddOnNewFrameCallback()
{
	if (v_dllHandle != NULL)
	{
		m_PopH264_DecoderAddOnNewFrameCallbackFromDLL = NULL;
		FString procName = "PopH264_DecoderAddOnNewFrameCallback";
		m_PopH264_DecoderAddOnNewFrameCallbackFromDLL = (_PopH264_DecoderAddOnNewFrameCallback)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
		if (m_getPopH264VersionFromDLL != NULL)
		{
			return true;
		}
	}

	return false;
}

bool ImportPopH264dll::Import_PopH264_CreateEncoder()
{
	if (v_dllHandle != NULL)
	{
		m_PopH264_CreateEncoderFromDLL = NULL;
		FString procName = "PopH264_GetVersion";
		m_PopH264_CreateEncoderFromDLL = (_PopH264_CreateEncoder)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
		if (m_PopH264_CreateEncoderFromDLL != NULL)
		{
			return true;
		}
	}

	return false;
}

bool ImportPopH264dll::Import_PopH264_DestroyEncoder()
{
	if (v_dllHandle != NULL)
	{
		m_PopH264_DestroyEncoderFromDLL = NULL;
		FString procName = "PopH264_DestroyEncoder";
		m_PopH264_DestroyEncoderFromDLL = (_PopH264_DestroyEncoder)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
		if (m_PopH264_DestroyEncoderFromDLL != NULL)
		{
			return true;
		}
	}

	return false;
}

bool ImportPopH264dll::Import_PopH264_EncoderPushFrame()
{
	if (v_dllHandle != NULL)
	{
		m_PopH264_EncoderPushFrameFromDLL = NULL;
		FString procName = "PopH264_EncoderPushFrame";
		m_PopH264_EncoderPushFrameFromDLL = (_PopH264_EncoderPushFrame)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
		if (m_PopH264_EncoderPushFrameFromDLL != NULL)
		{
			return true;
		}
	}

	return false;
}

bool ImportPopH264dll::Import_PopH264_EncoderEndOfStream()
{
	if (v_dllHandle != NULL)
	{
		m_PopH264_EncoderEndOfStreamFromDLL = NULL;
		FString procName = "PopH264_EncoderEndOfStream";
		m_PopH264_EncoderEndOfStreamFromDLL = (_PopH264_EncoderEndOfStream)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
		if (m_PopH264_EncoderEndOfStreamFromDLL != NULL)
		{
			return true;
		}
	}

	return false;
}

bool ImportPopH264dll::Import_PopH264_EncoderPopData()
{
	if (v_dllHandle != NULL)
	{
		m_PopH264_EncoderPopDataFromDLL = NULL;
		FString procName = "PopH264_EncoderPopData";
		m_PopH264_EncoderPopDataFromDLL = (_PopH264_EncoderPopData)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
		if (m_PopH264_EncoderPopDataFromDLL != NULL)
		{
			return true;
		}
	}

	return false;
}

bool ImportPopH264dll::Import_PopH264_EncoderPeekData()
{
	if (v_dllHandle != NULL)
	{
		m_PopH264_EncoderPeekDataFromDLL = NULL;
		FString procName = "PopH264_EncoderPeekData";
		m_PopH264_EncoderPeekDataFromDLL = (_PopH264_EncoderPeekData)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
		if (m_PopH264_EncoderPeekDataFromDLL != NULL)
		{
			return true;
		}
	}

	return false;
}

bool ImportPopH264dll::Import_PopH264_EncoderAddOnNewPacketCallback()
{
	if (v_dllHandle != NULL)
	{
		m_PopH264_EncoderAddOnNewPacketCallbackFromDLL = NULL;
		FString procName = "PopH264_EncoderAddOnNewPacketCallback";
		m_PopH264_EncoderAddOnNewPacketCallbackFromDLL = (_PopH264_EncoderAddOnNewPacketCallback)FPlatformProcess::GetDllExport(v_dllHandle, *procName);
		if (m_PopH264_EncoderAddOnNewPacketCallbackFromDLL != NULL)
		{
			return true;
		}
	}

	return false;
}

int32_t ImportPopH264dll::PopH264_GetVersion()
{
	if (m_getPopH264VersionFromDLL != NULL)
	{
		int32_t out = int(m_getPopH264VersionFromDLL());
		return out;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Function PopH264_GetVersion being called when not imported"));
	}
	return -32202;
}

void ImportPopH264dll::PopH264_Shutdown()
{
	if (m_PopH264_ShutdownFromDLL != NULL)
	{
		m_getPopH264VersionFromDLL();
		return;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Function PopH264_Shutdown being called when not imported"));
		return;
	}
}

int32_t ImportPopH264dll::PopH264_GetTestData(const char* Name, uint8_t* Buffer, int32_t BufferSize)
{
	if (m_PopH264_GetTestDataFromDLL != NULL)
	{
		int32_t out = int(m_PopH264_GetTestDataFromDLL(Name, Buffer, BufferSize));
		return out;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Function PopH264_GetTestData being called when not imported"));
	}
	return -32202;
}

int32_t ImportPopH264dll::PopH264_CreateDecoder(const char* OptionsJson, char* ErrorBuffer, int32_t ErrorBufferLength)
{
	if (m_PopH264_CreateDecoderFromDLL != NULL)
	{
		int32_t out = int(m_PopH264_CreateDecoderFromDLL(OptionsJson, ErrorBuffer, ErrorBufferLength));
		return out;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Function PopH264_CreateDecoder being called when not imported"));
	}
	return -32202;
}

void ImportPopH264dll::PopH264_DestroyDecoder(int32_t Instance)
{
	if (m_PopH264_DestroyDecoderFromDLL != NULL)
	{
		m_PopH264_DestroyDecoderFromDLL(Instance);
		return;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Function PopH264_DestroyDecoder being called when not imported"));
		return;
	}
}

void ImportPopH264dll::PopH264_DestroyInstance(int32_t Instance)
{
	if (m_PopH264_DestroyInstanceFromDLL != NULL)
	{
		m_PopH264_DestroyInstanceFromDLL(Instance);
		return;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Function PopH264_DestroyInstance being called when not imported"));
		return;
	}
}

void ImportPopH264dll::PopH264_EnumDecoders(char* DecodersJsonBuffer, int32_t DecodersJsonBufferLength)
{
	if (m_PopH264_EnumDecodersFromDLL != NULL)
	{
		m_PopH264_EnumDecodersFromDLL(DecodersJsonBuffer, DecodersJsonBufferLength);
		return;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Function PopH264_EnumDecoders being called when not imported"));
		return;
	}
}

void ImportPopH264dll::PopH264_GetMeta(int32_t Instance, int32_t* MetaValues, int32_t MetaValuesCount)
{
	if (m_PopH264_GetMetaFromDLL != NULL)
	{
		m_PopH264_GetMetaFromDLL(Instance, MetaValues, MetaValuesCount);
		return;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Function PopH264_GetMeta being called when not imported"));
		return;
	}
}

void ImportPopH264dll::PopH264_PeekFrame(int32_t Instance, char* JsonBuffer, int32_t JsonBufferSize)
{
	if (m_PopH264_PeekFrameFromDLL != NULL)
	{
		m_PopH264_PeekFrameFromDLL(Instance, JsonBuffer, JsonBufferSize);
		return;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Function PopH264_PeekFrame being called when not imported"));
		return;
	}
}

int32_t ImportPopH264dll::PopH264_PushData(int32_t Instance, uint8_t* Data, int32_t DataSize, int32_t FrameNumber)
{
	if (m_PopH264_PushDataFromDLL != NULL)
	{
		int32_t out = int32_t(m_PopH264_PushDataFromDLL(Instance, Data, DataSize, FrameNumber));
		return out;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Function PopH264_PushData being called when not imported"));
	}
	return -32202;
}

int32_t ImportPopH264dll::PopH264_PushEndOfStream(int32_t Instance)
{
	if (m_PopH264_PushEndOfStreamFromDLL != NULL)
	{
		int32_t out = int32_t(m_PopH264_PushEndOfStreamFromDLL(Instance));
		return out;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Function PopH264_PushEndOfStream being called when not imported"));
	}
	return -32202;
}

int32_t ImportPopH264dll::PopH264_PopFrame(int32_t Instance, uint8_t* Plane0, int32_t Plane0Size, uint8_t* Plane1, int32_t Plane1Size, uint8_t* Plane2, int32_t Plane2Size)
{
	if (m_PopH264_PopFrameFromDLL != NULL)
	{
		int32_t out = int32_t(m_PopH264_PopFrameFromDLL(Instance, Plane0, Plane0Size, Plane1, Plane1Size, Plane2, Plane2Size));
		return out;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Function PopH264_PopFrame being called when not imported"));
	}
	return -32202;
}

void ImportPopH264dll::PopH264_DecoderAddOnNewFrameCallback(int32_t Instance, PopH264_Callback* Callback, void* Meta)
{
	if (m_PopH264_DecoderAddOnNewFrameCallbackFromDLL != NULL)
	{
		m_PopH264_DecoderAddOnNewFrameCallbackFromDLL(Instance, Callback, Meta);
		return;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Function PopH264_DecoderAddOnNewFrameCallback being called when not imported"));
		return;
	}
}

int32_t ImportPopH264dll::PopH264_CreateEncoder(const char* OptionsJson, char* ErrorBuffer, int32_t ErrorBufferSize)
{
	if (m_PopH264_CreateEncoderFromDLL != NULL)
	{
		int32_t out = int32_t(m_PopH264_CreateEncoderFromDLL(OptionsJson, ErrorBuffer, ErrorBufferSize));
		return out;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Function PopH264_CreateEncoder being called when not imported"));
	}
	return -32202;
}

void ImportPopH264dll::PopH264_DestroyEncoder(int32_t Instance)
{
	if (m_PopH264_DestroyEncoderFromDLL != NULL)
	{
		m_PopH264_DestroyEncoderFromDLL(Instance);
		return;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Function PopH264_DestroyEncoder being called when not imported"));
		return;
	}
}

void ImportPopH264dll::PopH264_EncoderPushFrame(int32_t Instance, const char* MetaJson, const uint8_t* LumaData, const uint8_t* ChromaUData, const uint8_t* ChromaVData, char* ErrorBuffer, int32_t ErrorBufferSize)
{
	if (m_PopH264_EncoderPushFrameFromDLL != NULL)
	{
		m_PopH264_EncoderPushFrameFromDLL(Instance, MetaJson, LumaData, ChromaUData, ChromaVData, ErrorBuffer, ErrorBufferSize);
		return;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Function PopH264_EncoderPushFrame being called when not imported"));
		return;
	}
}

void ImportPopH264dll::PopH264_EncoderEndOfStream(int32_t Instance)
{
	if (m_PopH264_EncoderEndOfStreamFromDLL != NULL)
	{
		m_PopH264_EncoderEndOfStreamFromDLL(Instance);
		return;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Function PopH264_EncoderEndOfStream being called when not imported"));
		return;
	}
}

int32_t ImportPopH264dll::PopH264_EncoderPopData(int32_t Instance, uint8_t* DataBuffer, int32_t DataBufferSize)
{
	if (m_PopH264_EncoderPopDataFromDLL != NULL)
	{
		int32_t out = int32_t(m_PopH264_EncoderPopDataFromDLL(Instance, DataBuffer, DataBufferSize));
		return out;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Function PopH264_EncoderPopData being called when not imported"));
	}
	return -32202;
}

void ImportPopH264dll::PopH264_EncoderPeekData(int32_t Instance, char* MetaJsonBuffer, int32_t MetaJsonBufferSize)
{
	if (m_PopH264_EncoderPeekDataFromDLL != NULL)
	{
		m_PopH264_EncoderPeekDataFromDLL(Instance, MetaJsonBuffer, MetaJsonBufferSize);
		return;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Function PopH264_EncoderPeekData being called when not imported"));
		return;
	}
}

void ImportPopH264dll::PopH264_EncoderAddOnNewPacketCallback(int32_t Instance, PopH264_Callback* Callback, void* Meta)
{
	if (m_PopH264_EncoderAddOnNewPacketCallbackFromDLL != NULL)
	{
		m_PopH264_EncoderAddOnNewPacketCallbackFromDLL(Instance, Callback, Meta);
		return;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Function PopH264_EncoderAddOnNewPacketCallback being called when not imported"));
		return;
	}
}
