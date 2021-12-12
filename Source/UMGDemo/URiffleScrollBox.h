// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ScrollBox.h"
#include "URiffleScrollBox.generated.h"

/**
 * 
 */
UCLASS()
class UMGDEMO_API UURiffleScrollBox : public UScrollBox
{
	GENERATED_BODY()

public:
	bool GetMoveEnable();
	bool GetRunScroll();
	void SetRunScroll(bool IsScroll);
	int GetScrollSign();

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

protected:
	class SRiffleScrollBox* ScrollBoxPtr;
};
