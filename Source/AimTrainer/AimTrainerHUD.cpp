// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "AimTrainerHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

AAimTrainerHUD::AAimTrainerHUD()
{
	// Set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/FirstPerson/Textures/FirstPersonCrosshair"));
	CrosshairTex = CrosshairTexObj.Object;
	static ConstructorHelpers::FClassFinder<UUserWidget> TargetCounterClassFinder(TEXT("WidgetBlueprint'/Game/TargetCounter.TargetCounter_C'"));
	TargetCounterWidgetClass = TargetCounterClassFinder.Class;
	static ConstructorHelpers::FClassFinder<UUserWidget> TimerClassFinder(TEXT("WidgetBlueprint'/Game/Timer.Timer_C'"));
	TimerWidgetClass = TimerClassFinder.Class;

}

void AAimTrainerHUD::DrawHUD()
{
	Super::DrawHUD();

	// Draw very simple crosshair

	// find center of the Canvas
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	// draw the crosshair
	FCanvasTileItem TileItem( Center, CrosshairTex->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem( TileItem );
}

void AAimTrainerHUD::BeginPlay()
{
	Super::BeginPlay();
	if (TargetCounterWidgetClass)
		TargetCounter = CreateWidget<UUserWidget>(GetWorld(), TargetCounterWidgetClass);
	if (TargetCounter)
		TargetCounter->AddToViewport();
	if (TimerWidgetClass)
		Timer = CreateWidget<UUserWidget>(GetWorld(), TimerWidgetClass);
	if (Timer)
		Timer->AddToViewport();
}
