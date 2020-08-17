// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "AimTrainerGameMode.h"
#include "AimTrainerHUD.h"
#include "AimTrainerCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/TargetPoint.h"
#include "EngineUtils.h"
#include "Target.h"
#include "Engine/World.h"

AAimTrainerGameMode::AAimTrainerGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AAimTrainerHUD::StaticClass();

	static ConstructorHelpers::FObjectFinder<UBlueprint> TargetBlueprintFinder(TEXT("Blueprint'/Game/BP_Target.BP_Target'"));
	Target = (UClass*)TargetBlueprintFinder.Object->GeneratedClass;
}

void AAimTrainerGameMode::SpawnTarget()
{
	FVector spawnLocation = RandomVectorInRange(startPoint->GetActorLocation(), endPoint->GetActorLocation());
	FRotator rotation(0.0f, 0.0f, 0.0f);
	GetWorld()->SpawnActor<ATarget>(Target, spawnLocation, rotation);
}

void AAimTrainerGameMode::OnTargetHit()
{
	targetsHit++;
	SpawnTarget();
}

void AAimTrainerGameMode::BeginPlay()
{
	for (TActorIterator<ATargetPoint> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (ActorItr->GetName() == TEXT("StartSpawn"))
			startPoint = *ActorItr;
		else if (ActorItr->GetName() == TEXT("EndSpawn"))
			endPoint = *ActorItr;
	}
	if (startPoint && endPoint)
	{
		UE_LOG(LogTemp, Warning, TEXT("StartPoint position is %s, EndPoint is %s"), *startPoint->GetActorLocation().ToString(), *endPoint->GetActorLocation().ToString());
		SpawnTarget();
	}
}

FVector AAimTrainerGameMode::RandomVectorInRange(const FVector& startRange, const FVector& endRange) const
{
	float x = FMath::RandRange(startRange.X, endRange.X);
	float y = FMath::RandRange(startRange.Y, endRange.Y);
	float z = FMath::RandRange(startRange.Z, endRange.Z);
	return FVector(x, y, z);
}
