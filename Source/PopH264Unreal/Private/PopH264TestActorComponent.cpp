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
		UE_LOG( PopH264, Warning, TEXT( "Got new frame: %d x%d planes" ), Meta.FrameNumber, NewTextures.Num() );
	}
	else
	{
	}
}

