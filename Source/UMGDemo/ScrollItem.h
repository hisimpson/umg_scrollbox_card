// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScrollItem.generated.h"

/**
 * 
 */
UCLASS()
class UMGDEMO_API UScrollItem : public UUserWidget
{
	GENERATED_BODY()

public:
	UScrollItem(const FObjectInitializer& ObjectInitializer);
	void SetItem(FString str);

	void Data_SetText(FString str);
	FString& Data_GetText();

protected:
	virtual bool Initialize() override;

	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	//µ•¿Ã≈Õ
protected:
	UPROPERTY()
	FString ItemText;

	TMap<int, FVector2D> touchPoints;
};