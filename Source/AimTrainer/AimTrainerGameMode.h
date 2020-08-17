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
protected:
	virtual void BeginPlay() override;
private:
	FVector RandomVectorInRange(const FVector& startRange, const FVector& endRange) const;
	class ATargetPoint* startPoint;
	ATargetPoint* endPoint;
	UPROPERTY(EditAnywhere, Category = "Target")
	TSubclassOf<class ATarget> Target;
};



