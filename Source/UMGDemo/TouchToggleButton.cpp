// Fill out your copyright notice in the Description page of Project Settings.


#include "TouchToggleButton.h"
#include "Components/CheckBox.h"
#include "Styling/SlateTypes.h"
#include "Windows/WindowsPlatformTime.h"

UTouchToggleButton::UTouchToggleButton(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Init();
	ResetCheckButton();
	IsDoOnce = true;
}

void UTouchToggleButton::Init()
{
	ClickMethod = EButtonClickMethod::PreciseClick;
	TouchMethod = EButtonTouchMethod::PreciseTap;

	WidgetStyle.Normal.DrawAs = ESlateBrushDrawType::NoDrawType;
	WidgetStyle.Hovered.DrawAs = ESlateBrushDrawType::NoDrawType;
	WidgetStyle.Pressed.DrawAs = ESlateBrushDrawType::NoDrawType;
	WidgetStyle.Disabled.DrawAs = ESlateBrushDrawType::NoDrawType;

	OnClicked.AddDynamic(this, &UTouchToggleButton::OnClickButton);
}

void UTouchToggleButton::ResetCheckButton()
{
	if (MyCheckBox == nullptr)
		return;

	MyCheckBox->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UTouchToggleButton::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	if (IsDoOnce.IsSet() && IsDoOnce)
	{
		ResetCheckButton();
	}
}

void UTouchToggleButton::OnClickButton()
{
	double now = FPlatformTime::Seconds();
	if (now - LastClickTimer < 0.05f)
		return;

	LastClickTimer = now;

	UE_LOG(LogTemp, Log, TEXT("UTouchToggleButton::OnClickButton"));
	if (MyCheckBox)
	{
		if(MyCheckBox->GetCheckedState() == ECheckBoxState::Checked)
			MyCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
		else
			MyCheckBox->SetCheckedState(ECheckBoxState::Checked);
	}

	if (OnClickTouchButton.IsBound())
		OnClickTouchButton.Broadcast(this);
}