#include "Target.h"
#include "Components/StaticMeshComponent.h"

ATarget::ATarget()
{
	PrimaryActorTick.bCanEverTick = true;

	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	SetRootComponent(staticMesh);

}

void ATarget::BeginPlay()
{
	Super::BeginPlay();
	float scale = FMath::RandRange(0.4f, 1.5f);
	FVector scale3D(scale, scale, scale);
	SetActorScale3D(scale3D);
}

void ATarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

