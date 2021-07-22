// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
/*
#include "ProceduralMeshComponent.h"
#include "MediaPlayer.h"
#include "StreamMediaSource.h"
#include "cr/proto/models/volumetric.pb.h"
#include "MediaTexture.h"
#include "Materials/MaterialInstanceDynamic.h"
*/
#include "PopH264Unreal.h"
#include "PopH264TestActorComponent.generated.h"

#define CRPLAYERPLUGIN_API 

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRPLAYERPLUGIN_API UPopH264TestActorComponent : public UActorComponent {
    GENERATED_BODY()

private:
/*
    UPROPERTY(VisibleAnywhere)
    UProceduralMeshComponent* mesh_;
*/
	//UPROPERTY(VisibleAnywhere)
	//PopH264::Decoder* decoderInstance = nullptr;
	TUniquePtr<FPopH264DecoderInstance>	mDecoder;

public:	
	UPopH264TestActorComponent();

/*
	UPROPERTY(EditAnywhere, Category = "Video Texture")
	UMaterial* videoMat_;
	UPROPERTY(EditAnywhere, Category = "Video Texture")
	UMaterialInstanceDynamic* videoDynamicMat_;
	UPROPERTY(EditAnywhere, Category = "Video Texture")
	UMediaTexture* texture_;
*/
	//	having a UProperty means naked UObject-based pointers inside an array will get a strong pointer refernece
	UPROPERTY()
	TArray<UTexture2D*> mLastPlanes;
	bool	mHadFrame = false;	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void PostLoad() override;

	void		UpdateMaterial();

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
