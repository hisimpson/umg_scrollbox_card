// Fill out your copyright notice in the Description page of Project Settings.


#include "ScrollItem.h"
#include "Input/Reply.h"
#include "Components/TextBlock.h"

#pragma optimize( "", off ) 

UScrollItem::UScrollItem(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


bool UScrollItem::Initialize()
{
	if (Super::Initialize() == false)
		return false;

	return true;
}


void UScrollItem::SetItem(FString str)
{
	Data_SetText(str);
	UTextBlock* TextBlock = Cast<UTextBlock>(GetWidgetFromName("TextBlock_Text"));
	if (TextBlock)
		TextBlock->SetText(FText::FromString(str));
}

void UScrollItem::Data_SetText(FString str)
{
	ItemText = str;
}

FString& UScrollItem::Data_GetText()
{
	return ItemText;
}

FReply UScrollItem::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
#if  0
	touchPoints.Add(InGestureEvent.GetPointerIndex(), InGestureEvent.GetLastScreenSpacePosition());
	return FReply::Handled();
#else
	return Super::NativeOnTouchStarted(InGeometry, InGestureEvent);
#endif
}

FReply UScrollItem::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
#if 0
	if (!touchPoints.Contains(InGestureEvent.GetPointerIndex()))
		return FReply::Handled();

	FVector2D pre = touchPoints[InGestureEvent.GetPointerIndex()];
	FVector2D now = InGestureEvent.GetLastScreenSpacePosition();

	if (this->touchPoints.Num() == 1)
	{
		touchPoints.Add(InGestureEvent.GetPointerIndex(), InGestureEvent.GetLastScreenSpacePosition());
		FVector2D offset = now - pre;

		//Add your logic ...
	}
	else 
	{
		//ignore multi touch
		touchPoints.Add(InGestureEvent.GetPointerIndex(), InGestureEvent.GetLastScreenSpacePosition());
		return FReply::Handled();
	}

	return FReply::Handled();
#else
	return Super::NativeOnTouchStarted(InGeometry, InGestureEvent);
#endif
}

FReply UScrollItem::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
#if  0
	touchPoints.Remove(InGestureEvent.GetPointerIndex());
	return FReply::Handled();
#else
	return Super::NativeOnTouchEnded(InGeometry, InGestureEvent);
#endif
}

#pragma optimize( "", on ) 