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
	mDecoder->PushTestData("RainbowGradient.h264",FrameNumber);

	PopH264FrameMeta_t Meta;
	auto NewTextures = mDecoder->PopFrame(Meta);
	if ( NewTextures.Num() )
	{
		NewTextures[1]->UpdateResource();
		NewTextures[0]->UpdateResource();
		UE_LOG( PopH264, Warning, TEXT( "Got new frame: %d x%d planes" ), Meta.FrameNumber, NewTextures.Num() );

		AActor* Actor = GetOwner();
		TArray<UStaticMeshComponent*> Components;
		Actor->GetComponents<UStaticMeshComponent>(Components);
		for( int32 i=0; i<Components.Num(); i++ )
		{
			UStaticMeshComponent* StaticMeshComponent = Components[i];
			UStaticMesh* StaticMesh = StaticMeshComponent->GetStaticMesh();

			UMaterialInstanceDynamic* material = StaticMeshComponent->CreateDynamicMaterialInstance(0);
			if (material != nullptr)
			{
				material->SetTextureParameterValue(FName("Video"), NewTextures[0]);
				material->SetTextureParameterValue(FName("Video2"), NewTextures[1]);
			}
		}
	}
	else
	{
	}
	
}

