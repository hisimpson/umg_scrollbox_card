// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UMGDemoGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class UMGDEMO_API AUMGDemoGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AUMGDemoGameModeBase();
	virtual void StartPlay() override;

protected:
	TSubclassOf<class UUserWidget> UI_MainClass;

	UPROPERTY()
	class UUI_Main* UI_Main;
};
