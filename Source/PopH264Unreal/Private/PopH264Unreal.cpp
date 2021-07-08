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
		UE_LOG( LogTemp, Log, TEXT( "PopH264 version %d" ), Version );
		
		char DecodersJsonBuffer[1024] = {0};
		PopH264_EnumDecoders( DecodersJsonBuffer, sizeof(DecodersJsonBuffer) );
		//FString DecodersJsonString = FString::Printf( TEXT( "%s" ), DecodersJsonBuffer );
		FString DecodersJsonString = DecodersJsonBuffer;
		UE_LOG( LogTemp, Log, TEXT( "PopH264 Decoders: %s" ), *DecodersJsonString );
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
	if ( ExampleLibraryHandle )
	{
		FPlatformProcess::FreeDllHandle(ExampleLibraryHandle);
		ExampleLibraryHandle = nullptr;
	}
}

//	c++17
#if __cplusplus != 201703L && !defined(_HAS_CXX17)
namespace std
{
	//	implementation of c++17's std::size() to get safe C-array size
	template<typename T,size_t CARRAYSIZE>
	static int size(const T(&CArray)[CARRAYSIZE])
	{
		return CARRAYSIZE;
	}
}
#endif


TUniquePtr<FPopH264DecoderInstance> FPopH264DecoderInstance::AllocDecoder()
{
	auto Decoder = MakeUnique<FPopH264DecoderInstance>();
	return Decoder;
}

FPopH264DecoderInstance::FPopH264DecoderInstance()
{
	char ErrorBuffer[1024] = {0};
	const char* OptionsJson = nullptr;
	mInstanceHandle = PopH264_CreateDecoder( OptionsJson, ErrorBuffer, std::size(ErrorBuffer) );

	if ( mInstanceHandle <= 0 )
	{
		UE_LOG( LogTemp, Error, TEXT( "Failed to allocate PopH264 decoder(Handle %d); Error: %s" ), mInstanceHandle, ErrorBuffer );
	}
}

FPopH264DecoderInstance::~FPopH264DecoderInstance()
{
	PopH264_DestroyDecoder(mInstanceHandle);
}

void FPopH264DecoderInstance::PushH264Data(const TArray<uint8_t>& H264Data,size_t FrameNumber)
{
	auto* DataMutable = const_cast<uint8_t*>(H264Data.GetData());
	int32_t FrameNumber32 = FrameNumber;
	auto Error = PopH264_PushData( mInstanceHandle, DataMutable, H264Data.Num(), FrameNumber32 );

	//	gr: if this fails, probably shutdown (bad instance)
	if ( Error < 0 )
	{
		UE_LOG( LogTemp, Warning, TEXT( "PopH264_PushData() returned %d. InstanceHandle=%d" ), Error, mInstanceHandle );
	}
}

bool FPopH264DecoderInstance::PushTestData(const char* TestDataName,size_t FrameNumber)
{
	//	get some test data
	TArray<uint8_t> H264Data;
	H264Data.Init(0,50*1024);
	auto DataSize = PopH264_GetTestData(TestDataName, H264Data.GetData(), H264Data.Num() );
	if ( DataSize <= 0 )
	{
		UE_LOG( LogTemp, Warning, TEXT( "No PopH264 test data named %s" ), *TestDataName );
		return false;
	}
		
	auto AllowShrinking = true;
	H264Data.SetNum(DataSize,AllowShrinking);
	PushH264Data(H264Data,FrameNumber);

	return true;
}

TSharedPtr<FJsonObject> ParseJson(TArray<char>& JsonStringArray)
{
	//FString JsonString = ANSI_TO_TCHAR( JsonStringArray.GetData() );
	auto* JsonStringArrayCstr = JsonStringArray.GetData();
	FString JsonString(JsonStringArrayCstr);

	TSharedPtr<FJsonObject> ParsedJsonObject;  
	auto JsonReader = TJsonReaderFactory<>::Create(JsonString);
	
	if ( !FJsonSerializer::Deserialize(JsonReader, ParsedJsonObject) )
	{
		UE_LOG( LogTemp, Warning, TEXT( "Failed to deserialise frame JSON: %s" ), *JsonString );
		return nullptr;
	}

	//	get the parsed json as an object
	return ParsedJsonObject;
	/*
		Culture = JsonParsed[0]->AsObject()->GetStringField(FString("Culture"));
		MusicVolume = JsonParsed[1]->AsObject()->GetNumberField(FString("MusicVolume"));
		SoundVolume = JsonParsed[2]->AsObject()->GetNumberField(FString("SoundVolume"));
*/}

class PopH264FramePlaneMeta
{
public:
	void			ParseJson(FJsonObject& Json);

	EPixelFormat	mPixelFormat = PF_Unknown;
	int32_t			mWidth = 0;
	int32_t			mHeight = 0;
	int32_t			mDataSize = 0;
	int32_t			mChannels = 0;
};

class PopH264FrameMeta
{
public:
	void					ParseJson(FJsonObject& Json);

	FString					mError;
	FString					mDecoder;
	bool					mHardwareAccelerated = false;
	bool					mEndOfStream = false;
	int32_t					mFrameNumber = 0;
	int32_t					mFramesQueued = 0;
	int32_t					mRotation = 0;	//  clockwise rotation in degrees
	FString					mYuvColourMatrixName;
	int32_t					mAverageBitsPerSecondRate = 0;
	int32_t					mRowStrideBytes = 0;
	bool					mFlipped = false;
	bool					mImageWidth = false;
	bool					mImageHeight = false;
	//bool					mImageRect = false;	//	[x,y,w,h]
	
	PopH264FramePlaneMeta	mPlane0;
	PopH264FramePlaneMeta	mPlane1;
	PopH264FramePlaneMeta	mPlane2;
};

void PopH264FramePlaneMeta::ParseJson(FJsonObject& Json)
{
	mWidth = Json.GetIntegerField("Width");
	mHeight = Json.GetIntegerField("Height");
	mDataSize = Json.GetIntegerField("DataSize");
	mChannels = Json.GetIntegerField("Channels");

	auto FormatString = Json.GetStringField("Format");
	//	convert to UE format
}

void PopH264FrameMeta::ParseJson(FJsonObject& Json)
{
	mError = Json.GetStringField("Error");
	mDecoder = Json.GetStringField("Decoder");
	mHardwareAccelerated = Json.GetBoolField("HardwareAccelerated");
	mEndOfStream = Json.GetBoolField("EndOfStream");
	mFrameNumber = Json.GetIntegerField("FrameNumber");
	mFramesQueued = Json.GetIntegerField("FramesQueued");
	mRotation = Json.GetIntegerField("Rotation");
	mYuvColourMatrixName = Json.GetStringField("YuvColourMatrixName");
	mAverageBitsPerSecondRate = Json.GetIntegerField("AverageBitsPerSecondRate");
	mRowStrideBytes = Json.GetIntegerField("RowStrideBytes");
	mImageWidth = Json.GetIntegerField("ImageWidth");
	mImageHeight = Json.GetIntegerField("ImageHeight");

	auto JsonPlanes = Json.GetArrayField("Planes");
	if ( JsonPlanes.Num() >= 1 )
	{
		auto pPlaneJson = JsonPlanes[0]->AsObject();
		mPlane0.ParseJson(*pPlaneJson);
	}
	if ( JsonPlanes.Num() >= 2 )
	{
		auto pPlaneJson = JsonPlanes[1]->AsObject();
		mPlane1.ParseJson(*pPlaneJson);
	}
	if ( JsonPlanes.Num() >= 3 )
	{
		auto pPlaneJson = JsonPlanes[2]->AsObject();
		mPlane2.ParseJson(*pPlaneJson);
	}
}


UTexture2D* FPopH264DecoderInstance::PopFrame(PopH264FrameMeta_t& OutputMeta)
{
	//	peek frame, is there a new frame?
	TArray<char> FrameMetaString;
	//FrameMetaString.Init( '\0', 50 * 1024 );	<-- insanely slow
	FrameMetaString.SetNum(50*1024);
	/*
	FrameMetaString.Reserve(50 * 1024);
	for ( int i=0;	i<50 * 1024;	i++ )
		FrameMetaString.Add('\0');
		*/
	PopH264_PeekFrame( mInstanceHandle, FrameMetaString.GetData(), FrameMetaString.Num() );
	
	const char* FrameJsonReadable = FrameMetaString.GetData();
	
	//	parse json
	auto FrameMetaJson = ParseJson(FrameMetaString);
	//	error
	if ( !FrameMetaJson )
		return nullptr;
	PopH264FrameMeta FrameMeta;
	FrameMeta.ParseJson(*FrameMetaJson);
	
	//	no frame
	if ( FrameMeta.mFrameNumber < 0 )
		return nullptr;
	
	//	allocate data buffer for the frame
	TArray<uint8_t> Plane0Data;
	Plane0Data.Init(0x0, FrameMeta.mPlane0.mDataSize );
	TArray<uint8_t> Plane1Data;
	Plane0Data.Init(0x0, FrameMeta.mPlane1.mDataSize );
	TArray<uint8_t> Plane2Data;
	Plane0Data.Init(0x0, FrameMeta.mPlane2.mDataSize );
	
	auto PoppedFrameNumber = PopH264_PopFrame( mInstanceHandle, Plane0Data.GetData(), Plane0Data.Num(), Plane1Data.GetData(), Plane1Data.Num(), Plane2Data.GetData(), Plane2Data.Num() );
	if ( PoppedFrameNumber < 0 )
		return nullptr;

	//	todo: make textures
	UE_LOG( LogTemp, Warning, TEXT( "Todo: make textures" ) );
	return nullptr;	
	/*
	//	
	TSharedPtr<UTexture2D> pNewTexture( 
	//	update is async, so we buffer the new frame, update the texture, then free when done
	auto Width = 16;
	auto Height = 16;
UpdateTextureRegions
(
    int32 MipIndex,
    uint32 NumRegions,
    const FUpdateTextureRegion2D* ...,
    uint32 SrcPitch,
    uint32 SrcBpp,
    uint8* SrcData,
    TFunction< void*SrcData,...
)
*/
}





#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FPopH264UnrealModule, PopH264Unreal)


