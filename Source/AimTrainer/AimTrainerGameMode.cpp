// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "AimTrainerGameMode.h"
#include "AimTrainerHUD.h"
#include "AimTrainerCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/TargetPoint.h"
#include "EngineUtils.h"
#include "Target.h"
#include "Engine/World.h"
#include "Blueprint/UserWidget.h"
#include "TimerManager.h"
#include "Components/InputComponent.h"

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

	static ConstructorHelpers::FClassFinder<UUserWidget> EndMessageClassFinder(TEXT("WidgetBlueprint'/Game/EndMessage.EndMessage_C'"));
	EndMessageWidgetClass = EndMessageClassFinder.Class;
}

void AAimTrainerGameMode::OnTargetHit()
{
	targetsHit++;
	if (GetRoundTimeSeconds() >= roundLength)
		EndRound();
	else	
		SpawnTarget();
}

float AAimTrainerGameMode::GetRoundTimeSeconds()
{
	float roundTimeSeconds = GetWorld()->GetTimeSeconds() - roundStartTime;
	return roundTimeSeconds >= roundLength ? roundLength : roundTimeSeconds;
}

void AAimTrainerGameMode::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetFirstPlayerController()->InputComponent->BindAction("StartRound", IE_Pressed, this, &AAimTrainerGameMode::NewRound);
	FindSpawnPoints();
	if (startPoint && endPoint)
	{
		UE_LOG(LogTemp, Warning, TEXT("StartPoint position is %s, EndPoint is %s"), *startPoint->GetActorLocation().ToString(), *endPoint->GetActorLocation().ToString());
		StartRound();
	}
}

void AAimTrainerGameMode::FindSpawnPoints()
{
	for (TActorIterator<ATargetPoint> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (ActorItr->GetName() == TEXT("StartSpawn"))
			startPoint = *ActorItr;
		else if (ActorItr->GetName() == TEXT("EndSpawn"))
			endPoint = *ActorItr;
	}
}

void AAimTrainerGameMode::StartRound()
{
	targetsHit = 0;
	roundStartTime = GetWorld()->GetTimeSeconds();
	SpawnTarget();
	UE_LOG(LogTemp, Warning, TEXT("StartRound"))
}

void AAimTrainerGameMode::SpawnTarget()
{
	FVector spawnLocation = RandomVectorInRange(startPoint->GetActorLocation(), endPoint->GetActorLocation());
	GetWorld()->SpawnActor<ATarget>(Target, spawnLocation, FRotator());
}

FVector AAimTrainerGameMode::RandomVectorInRange(const FVector& startRange, const FVector& endRange) const
{
	float x = FMath::RandRange(startRange.X, endRange.X);
	float y = FMath::RandRange(startRange.Y, endRange.Y);
	float z = FMath::RandRange(startRange.Z, endRange.Z);
	return FVector(x, y, z);
}

void AAimTrainerGameMode::EndRound()
{
	bIsRoundOver = true;
	ShowEndMessage();
	//Wait for the user to press E
	GetWorldTimerManager().SetTimer(InputTimer, this, &AAimTrainerGameMode::WaitForInput, 0.5f, true, 0.5f);
}

void AAimTrainerGameMode::ShowEndMessage()
{
	if (EndMessageWidgetClass)
		EndMessage = CreateWidget<UUserWidget>(GetWorld(), EndMessageWidgetClass);
	if (EndMessage)
		EndMessage->AddToViewport();
}

void AAimTrainerGameMode::NewRound()
{
	bIsRoundOver = false;
}


void AAimTrainerGameMode::WaitForInput()
{
	if (!bIsRoundOver)
	{
		EndMessage->RemoveFromViewport();
		StartRound();
		GetWorldTimerManager().ClearTimer(InputTimer);
	}
	UE_LOG(LogTemp, Warning, TEXT("TimerTick"))
}