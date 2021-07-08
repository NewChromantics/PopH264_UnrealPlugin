#include "PopH264TestActorComponent.h"
#include "PopH264Unreal.h"


UPopH264TestActorComponent::UPopH264TestActorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	//mesh_ = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
}


/*
void UPopH264TestActorComponent::AssignPlayerAndTextures(UMediaPlayer* _player,
                                                   UStreamMediaSource* _streamSource,
                                                   UMaterial* _material) {
    videoMat_ = _material;
    streamSource_ = _streamSource;
    mediaPlayer_ = _player;
    //    m_streamSource->StreamUrl = "https://storage.googleapis.com/streamsvc-publish-uc7x7z/v0.1/1043/textures-00000000000000000001.mp4";
}
*/
// Called when the game starts
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
	mDecoder->PushTestData("Rainbow.h264",FrameNumber);

	PopH264FrameMeta_t Meta;
	auto* NewTexture = mDecoder->PopFrame(Meta);
	if ( NewTexture )
	{
		UE_LOG( LogTemp, Warning, TEXT( "Got new frame: %d" ), Meta.FrameNumber );
	}
}

