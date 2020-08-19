// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AimTrainerGameMode.generated.h"
UCLASS(minimalapi)
class AAimTrainerGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AAimTrainerGameMode();
	void SpawnTarget();
	void StartRound();
	void EndRound();
	void NewRound();
	void ShowEndMessage();
	void WaitForInput();
	void OnTargetHit();
	UFUNCTION(BlueprintCallable)
	float GetRoundTimeSeconds();
	UPROPERTY(BlueprintReadOnly)
	int32 targetsHit = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float roundLength = 60.0f;
protected:
	virtual void BeginPlay() override;
private:
	FVector RandomVectorInRange(const FVector& startRange, const FVector& endRange) const;
	class ATargetPoint* startPoint;
	ATargetPoint* endPoint;
	UPROPERTY(EditAnywhere, Category = "Target")
	TSubclassOf<class ATarget> Target;
	float roundStartTime = 0.0f;
	bool bIsRoundOver = false;
	struct FTimerHandle InputTimer;
	UPROPERTY(EditAnywhere, Category = "Interface")
		TSubclassOf<class UUserWidget> EndMessageWidgetClass;
	class UUserWidget* EndMessage;

};



