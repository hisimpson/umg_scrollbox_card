// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TouchScrollWidget.h"
#include "MyScrollWidget.generated.h"

/**
 * 
 */
UCLASS()
class UMGDEMO_API UMyScrollWidget : public UTouchScrollWidget
{
	GENERATED_BODY()
	
public:
	UMyScrollWidget(const FObjectInitializer& ObjectInitializer);
	void AddItem(FString str, const FVector2D& Scale);
	virtual void ClickItem(UUserWidget* UserWidget) override;

protected:
	virtual bool Initialize() override;

};
