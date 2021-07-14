// Copyright Epic Games, Inc. All Rights Reserved.

#include "PopH264Unreal.h"
#include "Core.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"
#include "GenericPlatform/GenericPlatformMath.h"

//	gr: todo: add include path to plugin build.cs
#include "PopH264/windows/Release_x64/PopH264.h"
#include <iterator>

#define LOCTEXT_NAMESPACE "FPopH264UnrealModule"


DEFINE_LOG_CATEGORY(PopH264)


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
		UE_LOG( PopH264, Log, TEXT( "PopH264 version %d" ), Version );
		
		char DecodersJsonBuffer[1024] = {0};
		PopH264_EnumDecoders( DecodersJsonBuffer, sizeof(DecodersJsonBuffer) );
		//FString DecodersJsonString = FString::Printf( TEXT( "%s" ), DecodersJsonBuffer );
		FString DecodersJsonString = DecodersJsonBuffer;
		UE_LOG( PopH264, Log, TEXT( "PopH264 Decoders: %s" ), *DecodersJsonString );
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
		UE_LOG( PopH264, Error, TEXT( "Failed to allocate PopH264 decoder(Handle %d); Error: %s" ), mInstanceHandle, ErrorBuffer );
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
		UE_LOG( PopH264, Warning, TEXT( "PopH264_PushData() returned %d. InstanceHandle=%d" ), Error, mInstanceHandle );
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
		UE_LOG( PopH264, Warning, TEXT( "Failed to deserialise frame JSON: %s" ), *JsonString );
		return nullptr;
	}

	//	get the parsed json as an object
	return ParsedJsonObject;
}


//	wrappers to suppress errors/warnings for missing keys (which don't mention the keys)
void ReadJsonValue(FJsonObject& Json,int32_t& Value,const char* Key)
{
	if ( !Json.HasField(Key) )
		return;
	Value = Json.GetIntegerField(Key);
}

void ReadJsonValue(FJsonObject& Json,bool& Value,const char* Key)
{
	if ( !Json.HasField(Key) )
		return;
	Value = Json.GetBoolField(Key);
}

void ReadJsonValue(FJsonObject& Json,FString& Value,const char* Key)
{
	if ( !Json.HasField(Key) )
		return;
	Value = Json.GetStringField(Key);
}



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
	int32_t					mImageWidth = false;
	int32_t					mImageHeight = false;
	//int32_tx4					mImageRect = false;	//	[x,y,w,h]
	
	PopH264FramePlaneMeta	mPlane0;
	PopH264FramePlaneMeta	mPlane1;
	PopH264FramePlaneMeta	mPlane2;
};


EPixelFormat GetPixelFormat(int ComponentCount,const FString& FormatName)
{
	//	todo: try and take advantage of PF_UYVY
	
	//	catch special cases (especiall non 8bit!)
	if ( FormatName == "Float1" )
		return PF_R32_FLOAT;
	if ( FormatName == "Float3" )
		return PF_FloatRGB;
	if ( FormatName == "Float4" )
		return PF_FloatRGBA;
	
	if ( FormatName == "ARGB" )
		return PF_A8R8G8B8;
	if ( FormatName == "BGRA" )
		return PF_B8G8R8A8;
	
	//	Arthur: I might be getting the wrong values here but it seemed closest to the formats in Unity
	switch (ComponentCount)
	{
		case 1: return PF_R8;
		case 2: return PF_R8G8;
		//case 3: return PF_R8G8B8;	//	no 24bit format
		case 4:	return PF_R8G8B8A8;
		
		default:
			UE_LOG(PopH264, Warning, TEXT("Don't know what format to use for component count %i (%s)"), ComponentCount, *FormatName );
			return PF_Unknown;
	}
}


void PopH264FramePlaneMeta::ParseJson(FJsonObject& Json)
{
	ReadJsonValue( Json, mWidth, "Width" ); 
	ReadJsonValue( Json, mHeight, "Height" ); 
	ReadJsonValue( Json, mDataSize, "DataSize" ); 
	ReadJsonValue( Json, mChannels, "Channels" ); 
	
	//	convert string format to UE format
	FString FormatString;
	ReadJsonValue( Json, FormatString, "Format" );
	//	gr: format will be like ChromaU, Luma, Depthf etc
	//	so maybe easier to get the DATA format via components (we do this in unity)
	mPixelFormat = GetPixelFormat(mChannels,FormatString);
}


void PopH264FrameMeta::ParseJson(FJsonObject& Json)
{
	ReadJsonValue( Json, mError, "Error");
	ReadJsonValue( Json, mDecoder, "Decoder");
	ReadJsonValue( Json, mHardwareAccelerated, "HardwareAccelerated");
	ReadJsonValue( Json, mEndOfStream, "EndOfStream");
	ReadJsonValue( Json, mFrameNumber, "FrameNumber");
	ReadJsonValue( Json, mFramesQueued, "FramesQueued");
	ReadJsonValue( Json, mRotation, "Rotation");
	ReadJsonValue( Json, mYuvColourMatrixName, "YuvColourMatrixName");
	ReadJsonValue( Json, mAverageBitsPerSecondRate, "AverageBitsPerSecondRate");
	ReadJsonValue( Json, mRowStrideBytes, "RowStrideBytes");
	ReadJsonValue( Json, mImageWidth, "ImageWidth");
	ReadJsonValue( Json, mImageHeight, "RowStrideBytes");
	ReadJsonValue( Json, mRowStrideBytes, "ImageHeight");

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


TArray<UTexture2D*> FPopH264DecoderInstance::PopFrame(PopH264FrameMeta_t& OutputMeta)
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
	//	error parsing
	if ( !FrameMetaJson )
		return TArray<UTexture2D*>();
		
	PopH264FrameMeta FrameMeta;
	FrameMeta.ParseJson(*FrameMetaJson);
	
	//	no new frame
	if ( FrameMeta.mFrameNumber < 0 )
		return TArray<UTexture2D*>();
	
	//	allocate data buffer for the frame
	//	gr: can we use BulkData, and can we use the bulk data directly from a texture...
	TArray<uint8_t> Plane0Data;
	Plane0Data.SetNum(FrameMeta.mPlane0.mDataSize );
	TArray<uint8_t> Plane1Data;
	Plane1Data.SetNum(FrameMeta.mPlane1.mDataSize );
	TArray<uint8_t> Plane2Data;
	Plane2Data.SetNum(FrameMeta.mPlane2.mDataSize );
	
	auto PoppedFrameNumber = PopH264_PopFrame( mInstanceHandle, Plane0Data.GetData(), Plane0Data.Num(), Plane1Data.GetData(), Plane1Data.Num(), Plane2Data.GetData(), Plane2Data.Num() );
	if ( PoppedFrameNumber < 0 )
		return TArray<UTexture2D*>();

	//	gr: we could allocate textures early, then pass the Raw BulkData into the plugin and write directly
	//		to save a copy
	TArray<UTexture2D*> Textures;
	auto MakeTexture = [&](TArray<uint8_t>& PlaneBytes,PopH264FramePlaneMeta& PlaneMeta,int PlaneIndex) -> UTexture2D*
	{
		if ( PlaneBytes.Num() == 0 )
			return nullptr;

		auto Width = PlaneMeta.mWidth;
		auto Height = PlaneMeta.mHeight;
		auto TextureNameString = FString::Printf( TEXT( "PopH264 #%d Plane%d %dx%d" ), PoppedFrameNumber, PlaneIndex, Width, Height );
		FName TextureName( *TextureNameString );

		UTexture2D* pTexture = UTexture2D::CreateTransient( Width, Height, PlaneMeta.mPixelFormat, TextureName );
		if ( !pTexture )
		{
			UE_LOG( LogTemp, Error, TEXT( "Failed to allocate plane texture %dx%d" ), PlaneMeta.mWidth, PlaneMeta.mHeight );
			return nullptr;
		}
		auto& Texture = *pTexture;
		if ( !Texture.PlatformData )
		{
			UE_LOG( PopH264, Error, TEXT( "Allocated texture but no .PlatformData (leaking texture?)" ) );
			return nullptr;
		}
		FTexture2DMipMap& TextureMip = Texture.PlatformData->Mips[0];
		uint8_t* MipPixels = static_cast<uint8_t*>( TextureMip.BulkData.Lock(LOCK_READ_WRITE) );
		if ( !MipPixels )
		{
			UE_LOG( PopH264, Error, TEXT( "Failed to lock mip0 pixels for plane %dx%d (leaking texture?)" ), PlaneMeta.mWidth, PlaneMeta.mHeight );
			return nullptr;
		}
		//	GetElementCount()
		auto MipPixelsSize = TextureMip.BulkData.GetBulkDataSize();
		
		//	do a safe copy
		auto CopySize = FGenericPlatformMath::Min<int32_t>( PlaneBytes.Num(), MipPixelsSize );
		auto* Source = PlaneBytes.GetData();
		auto* Dest = MipPixels;
		//FMemory::BigBlockMemcpy any different?
		FMemory::Memcpy( Dest, Source, CopySize );

		TextureMip.BulkData.Unlock();
		Texture.UpdateResource();
		return pTexture;
	};
	auto MakeAndAddTexture = [&](TArray<uint8_t>& PlaneBytes,PopH264FramePlaneMeta& PlaneMeta,int PlaneIndex)
	{
		auto* Texture = MakeTexture( PlaneBytes, PlaneMeta, PlaneIndex );
		if ( !Texture )
			return;
		Textures.Push(Texture);
	};
	MakeAndAddTexture( Plane0Data, FrameMeta.mPlane0, 0 );
	MakeAndAddTexture( Plane1Data, FrameMeta.mPlane1, 1 );
	MakeAndAddTexture( Plane2Data, FrameMeta.mPlane2, 2 );
	
	return Textures;
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


