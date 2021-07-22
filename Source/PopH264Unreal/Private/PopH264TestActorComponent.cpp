#include "PopH264TestActorComponent.h"
#include "PopH264Unreal.h"


UPopH264TestActorComponent::UPopH264TestActorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPopH264TestActorComponent::BeginPlay()
{
	Super::BeginPlay();
	
	mDecoder = FPopH264DecoderInstance::AllocDecoder();
}

void UPopH264TestActorComponent::PostLoad() 
{ 
	Super::PostLoad();
}


void UPopH264TestActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	size_t FrameNumber = 999;
	static int x = 0;
	const char* TestDataName = ((x++) & 1) ? "GreyscaleGradient.h264" : "RainbowGradient.h264";
	mDecoder->PushTestData(TestDataName, FrameNumber);

	//if (!mHadFrame)
	{
		PopH264FrameMeta_t Meta;
		auto NewTextures = mDecoder->PopFrame(Meta,mLastPlanes);
		if (NewTextures.Num())
		{
			UE_LOG(PopH264, Log, TEXT("Got new frame: %d x%d planes"), Meta.FrameNumber, NewTextures.Num());
			mLastPlanes = NewTextures;
			mHadFrame = true;
		}
	}

	UpdateMaterial();
}

void UPopH264TestActorComponent::UpdateMaterial()
{
	//	todo: use a safe array type
	static const FName MaterialParameterNames[] = { FName("Video"), FName("Video2") , FName("Video3") };

	AActor* Actor = GetOwner();
	TArray<UStaticMeshComponent*> Components;
	Actor->GetComponents<UStaticMeshComponent>(Components);
	for( int32 i=0; i<Components.Num(); i++ )
	{
		UStaticMeshComponent* StaticMeshComponent = Components[i];
		UStaticMesh* StaticMesh = StaticMeshComponent->GetStaticMesh();

		UMaterialInstanceDynamic* material = StaticMeshComponent->CreateDynamicMaterialInstance(0);
		if (!material)
			continue;

		//	gr: set a param to null if no plane for that index?
		for (auto t = 0; t < mLastPlanes.Num(); t++)
		{
			auto* Texture = mLastPlanes[t];
			auto ParameterName = MaterialParameterNames[t];

			if ( Texture )
				UE_LOG(PopH264, Log, TEXT("Set material plane %d texture"), t);

			material->SetTextureParameterValue(ParameterName, Texture);
		}
	}
	
}

