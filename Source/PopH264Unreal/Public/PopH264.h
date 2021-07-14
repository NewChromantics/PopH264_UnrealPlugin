// Copyright (C) Condense Reality Ltd - All Rights Reserved * Unauthorized copying of this file, via any medium is strictly prohibited * Proprietary and confidential * Written by Arthur Muddiman<arthur@condensereality.com>, March 2021


#pragma once

#include "CoreMinimal.h"
//#include "PopH264Unreal.h"
//#include "JsonObjectConverter.h"
#include "ImportPopH264dll.h"
#include "Serialization/JsonWriter.h"

#include "PopH264/windows/Release_x64/PopH264.h"

/*
class CRPLAYERPLUGIN_API PopH264
{
public:

	//	gr: these numbers don't matter in PopH264, need a better way to map these across depedencies
	//		other than matching strings
	//	for use with PopYuv shader, these enum values should match the shader
	enum PixelFormat
	{
		Debug = 999,
		Invalid = 0,
		Greyscale = 1,
		RGB = 2,
		RGBA = 3,
		BGRA = 4,
		BGR = 5,
		YYuv_8888_Full = 6,
		YYuv_8888_Ntsc = 7,
		Depth16mm = 8,
		Chroma_U = 9,
		Chroma_V = 10,
		ChromaUV_88 = 11,
		ChromaVU_88 = 12,
		Luma_Ntsc = 13,


		ChromaU_8 = Chroma_U,
		ChromaV_8 = Chroma_V,
	};

	struct PlaneMeta
	{
	public:
		PixelFormat		PixelFormat; // TODO: Must write some form of getter{ get { return (PixelFormat)Enum.Parse(typeof(PixelFormat), Format); } };
		FString			Format;
		int				Width;
		int				Height;
		int				DataSize;
		int				Channels;
	};

	USTRUCT()
	struct FrameMeta
	{
	public:
		TArray<PlaneMeta>	Planes;
		int				PlaneCount; // { get{ return Planes != null ? Planes.Count : 0; } }

		FString			Error;
		FString			Decoder;				//	internal name of codec (if provided by API/OS)
		bool			HardwareAccelerated;	//	are we using a hardware accelerated decoder. DO NOT rely on this information as if not provided cs defaults to false. Currently MediaFoundation only
		bool			EndOfStream;
		int 			FrameNumber;
		int				FramesQueued;	//	number of subsequent frames already decoded and buffered up

		// optional meta output by decoder
		int				Rotation;   //  clockwise rotation in degrees
		FString			YuvColourMatrixName;    //	todo: enum this
		int				AverageBitsPerSecondRate;
		int				RowStrideBytes;
		bool			Flipped;
		int				ImageWidth;
		int				ImageHeight;
		int				ImageRect[];		//	some decoders will output an image aligned to say, 16 (macro blocks, or byte alignment etc) If the image is padded, we should have a [x,y,w,h] array here
	};

	USTRUCT() struct FrameInput
	{
	public:
		UPROPERTY()
		TArray<uint8> Bytes;
		UPROPERTY()
		int FrameNumber;

		bool GetEndOfStream()
		{
			return Bytes.GetData() == nullptr;
		}	//	marker/command to tell decoder stream has ended
	};

	USTRUCT()
	struct DecoderParams
	{
		//	Avf, Broadway, MediaFoundation, MagicLeap, Intel etc
		//	empty string defaults to "best" (hardware where possible)
		//	todo: PopH264_EnumDecoders which will return a list of all possible decoders
		//	ie. low level specific decoders/codecs installed on the system, including say MediaFoundation_NvidiaHardwareH264, or MagicLeap_GoogleSoftware
		UPROPERTY()
		FString Decoder;

		//	print extra debug info (all decoders)
		UPROPERTY()
		bool VerboseDebug;

		UPROPERTY()
		bool AllowBuffering;			//	by default poph264 tries to reduce amount of buffering decoders do and deliver frames ASAP
		UPROPERTY()
		bool DoubleDecodeKeyframe;	//	Hack for broadway & MediaFoundation, process a keyframe twice to instantly decode instead of buffering
		UPROPERTY()
		bool DrainOnKeyframe;		//	Debug for MediaFoundation, trigger drain command on a keyfrae
		UPROPERTY()
		bool LowPowerMode;
		UPROPERTY()
		bool DropBadFrames;
		UPROPERTY()
		bool DecodeSei;

		//	gr: only set these for testing. 0 means no hint will be set
		//public int	Width;
		//public int	Height;
		//public int	InputSize;
	};


	class Decoder
	{
	protected:
		int32_t Instance;
		ImportPopH264dll popH264dll;
		
		TArray<TArray<uint8>> PlaneCaches;
		TArray<uint8> UnusedBuffer;
		bool ThreadedDecoding = true;
		TArray<FrameInput> InputQueue;
		bool InputThreadResult;
		bool HadEndOfStream = false;

		TArray<uint8> JsonBufferPrealloc;

	public:
		Decoder();
		Decoder(DecoderParams _DecoderParams, bool _ThreadedDecoding);
		~Decoder();
		void Dispose();
		void GetNextFrameAndMeta(TArray<UTexture2D*> Planes, TArray<PixelFormat> PixelFormats, FrameMeta& OutValue);
		UTexture2D* AllocTexture(UTexture2D* Plane, PlaneMeta Meta);
		UMaterialInstanceDynamic* GetDecodedFrame();

	protected:
		template <typename T>
		void AllocListToSize(TArray<T> Array, int size);
		EPixelFormat GetTextureFormat(int ComponentCount);
	};


	PopH264();
	~PopH264();
};

template<typename T>
inline void PopH264::Decoder::AllocListToSize(TArray<T> Array, int size)
{
	while (Array.Num() < size)
	{
		Array.Add(T());
	}
}
*/
