// Copyright (C) Condense Reality Ltd - All Rights Reserved * Unauthorized copying of this file, via any medium is strictly prohibited * Proprietary and confidential * Written by Arthur Muddiman<arthur@condensereality.com>, March 2021


#include "PopH264.h"

/*
PopH264::PopH264()
{
}

PopH264::~PopH264()
{
}

PopH264::Decoder::Decoder(DecoderParams _DecoderParams, bool _ThreadedDecoding)
{
	int32_t Version = popH264dll.PopH264_GetVersion();
	UE_LOG(LogTemp, Warning, TEXT("PopH264 version %i"), Version);

	this->ThreadedDecoding = _ThreadedDecoding;

	if (&_DecoderParams == nullptr)
		_DecoderParams = PopH264::DecoderParams();

	TArray<uint8> ErrorBuffer;

	//TODO: There seem to be issues with the use of json so I can't create an instance of PopH264 in this project.
	//Weirdly, this isn't a problem in my project that's forked from the main PopH264 repo
	
	/ * FString OutJson;
	FJsonObjectConverter::UStructToJsonObjectString(_DecoderParams, OutJson, 0, 0);
	Instance = popH264dll.PopH264_CreateDecoder(TCHAR_TO_ANSI(*OutJson), (char*)ErrorBuffer.GetData(), ErrorBuffer.Num());
	UE_LOG(LogTemp, Warning, TEXT("Instance created"));*/
	/*
}

void PopH264::Decoder::Dispose()
{
	PopH264_DestroyDecoder(Instance);
}

void PopH264::Decoder::GetNextFrameAndMeta(TArray<UTexture2D*> Planes, TArray<PixelFormat> PixelFormats, FrameMeta& OutValue)
{
	PopH264_PeekFrame(Instance, (char*)JsonBufferPrealloc.GetData(), JsonBufferPrealloc.Num());

	//Some json stuff that I can't get working...

	int PlaneCount = OutValue.PlaneCount;

	if (OutValue.EndOfStream)
		HadEndOfStream = true;

	if (PlaneCount <= 0)
	{
		return;
	}

	if (OutValue.FrameNumber < 0)
		return;

	AllocListToSize(Planes, PlaneCount);
	AllocListToSize(PixelFormats, PlaneCount);
	AllocListToSize(PlaneCaches, PlaneCount);

	if (PlaneCount >= 1) PixelFormats[0] = OutValue.Planes[0].PixelFormat;
	if (PlaneCount >= 2) PixelFormats[1] = OutValue.Planes[1].PixelFormat;
	if (PlaneCount >= 3) PixelFormats[2] = OutValue.Planes[2].PixelFormat;

	//	alloc textures so we have data to write to
	if (PlaneCount >= 1) AllocTexture(Planes[0], OutValue.Planes[0]);
	if (PlaneCount >= 2) AllocTexture(Planes[1], OutValue.Planes[1]);
	if (PlaneCount >= 3) AllocTexture(Planes[2], OutValue.Planes[2]);
	
	for (int p = 0; p < PlaneCount; p++)
	{
		if (PlaneCaches[p].GetData() != nullptr)
			continue;
		if (!Planes[p])
			continue;

		uint8* raw = NULL;
		FTexture2DMipMap& Mip = Planes[p]->PlatformData->Mips[0];
		void* Data = Mip.BulkData.Lock(LOCK_READ_WRITE);
		raw = (uint8*)Data;

		TArray<uint8> RawAsArray;
		RawAsArray.Add(*raw);

		PlaneCaches[p] = RawAsArray;
	}

	auto Plane0Data = (PlaneCaches.Num() >= 1 && PlaneCaches[0].GetData() != nullptr) ? PlaneCaches[0] : UnusedBuffer;
	auto Plane1Data = (PlaneCaches.Num() >= 2 && PlaneCaches[1].GetData() != nullptr) ? PlaneCaches[1] : UnusedBuffer;
	auto Plane2Data = (PlaneCaches.Num() >= 3 && PlaneCaches[2].GetData() != nullptr) ? PlaneCaches[2] : UnusedBuffer;

	auto PopResult = PopH264_PopFrame(Instance, Plane0Data.GetData(), Plane0Data.Num(), Plane1Data.GetData(), Plane1Data.Num(), Plane2Data.GetData(), Plane2Data.Num());
	if (PopResult < 0)
	{
		return;
	}

	for (int i = 0; i < PlaneCount; i++)
	{
		if (!Planes[i])
			continue;

	}

	OutValue.FrameNumber = PopResult;
}

UTexture2D* PopH264::Decoder::AllocTexture(UTexture2D* Plane, PlaneMeta Meta)
{
	auto Format = GetTextureFormat(Meta.Channels);

	if (Plane != nullptr)
		if (Plane->GetSurfaceWidth() != Meta.Width)
			return nullptr;
		else if (Plane->GetSurfaceHeight() != Meta.Height)
			return nullptr;
		else if (Plane->GetPixelFormat() != Format)
			return nullptr;

	if (!Plane)
	{
		bool MipMap = false;
		bool Liner = true;
		Plane = UTexture2D::CreateTransient(Meta.Width, Meta.Height, Format);
		Plane->Filter = TF_Default;
	}

	return Plane;
}

UMaterialInstanceDynamic* PopH264::Decoder::GetDecodedFrame()
{
	UMaterial* baseMat = NewObject<UMaterial>();
	UMaterialInstanceDynamic* mat = UMaterialInstanceDynamic::Create(baseMat, nullptr);
	mat->SetScalarParameterValue(TEXT("EmissiveStrength"), 0);
	mat->SetVectorParameterValue(TEXT("Color"), FLinearColor::Yellow);
	return mat;
}

EPixelFormat PopH264::Decoder::GetTextureFormat(int ComponentCount)
{
	//I might be getting the wrong values here but it seemed closest to the formats in Unity
	switch (ComponentCount)
	{
		case 1: return PF_R8;
		case 2: return PF_R8G8;
		case 3: return PF_FloatRGB;
		case 4: return PF_FloatRGBA;
		default:
			UE_LOG(LogTemp, Error, TEXT("Don't know what format to use for component count %i"), ComponentCount);
			return PF_Unknown;
	}
}



PopH264::Decoder::~Decoder()
{
	Dispose();
}

*/
