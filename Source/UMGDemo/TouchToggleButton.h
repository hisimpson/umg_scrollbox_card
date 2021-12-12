// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "TouchToggleButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FClickTouchButton, UWidget*, Widget);

/**
 * 
 */
UCLASS()
class UMGDEMO_API UTouchToggleButton : public UButton
{
	GENERATED_BODY()
	
public:
	UTouchToggleButton(const FObjectInitializer& ObjectInitializer);
	virtual void SynchronizeProperties() override;
	FClickTouchButton OnClickTouchButton;

protected:
	void Init();
	void ResetCheckButton();

	UFUNCTION()
	void OnClickButton();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ToggleButton", meta = (DisplayName = "CheckBox", AllowPrivateAccess = "true"))
	class UCheckBox* MyCheckBox;

	double LastClickTimer = 0.0f;

	TOptional<bool> IsDoOnce;

};
