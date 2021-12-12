// Fill out your copyright notice in the Description page of Project Settings.


#include "URiffleScrollBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Components/ScrollBoxSlot.h"

namespace GlobalSetting
{
	bool TouchScreen = false;

	void SetTouchScreen( bool Flag)
	{
		TouchScreen = Flag;
	}

	bool UseTouchScreen()
	{
		return TouchScreen;
	}
}

class UMGDEMO_API SRiffleScrollBox : public SScrollBox
{
protected:
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	//virtual FReply OnTouchGesture(const FGeometry& MyGeometry, const FPointerEvent& GestureEvent) override;
	virtual FReply OnTouchStarted(const FGeometry& MyGeometry, const FPointerEvent& InTouchEvent) override;
	virtual FReply OnTouchMoved(const FGeometry& MyGeometry, const FPointerEvent& InTouchEvent) override;
	virtual FReply OnTouchEnded(const FGeometry& MyGeometry, const FPointerEvent& InTouchEvent) override;

	float GetDiffOffset(const FPointerEvent& InEvent);

public:
	bool GetMoveEnable()
	{
		return MoveEnable;
	}

	bool GetRunScroll()
	{
		return RunScroll;
	}

	void SetRunScroll(bool IsScroll)
	{
		RunScroll = IsScroll;
	}

	int GetScrollSign()
	{
		return ScrollSign;
	}

protected:
	FVector2D Begin;
	bool RunScroll = false;
	bool MoveEnable = false;
	int ScrollSign = 0;
};

FReply SRiffleScrollBox::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if(GlobalSetting::UseTouchScreen())
		return FReply::Handled();

	if(!MouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
		return FReply::Handled();

	RunScroll = false;
	MoveEnable = false;
	ScrollSign = 0;
	Begin = MouseEvent.GetScreenSpacePosition();
	return FReply::Handled();
}

FReply SRiffleScrollBox::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (GlobalSetting::UseTouchScreen())
		return FReply::Handled();

	if (RunScroll)
		return FReply::Handled();

	if (!MouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
		return FReply::Handled();

	float diff = GetDiffOffset(MouseEvent);
	if (FMath::Abs(diff) > 20.0f)
	{
		MoveEnable = true;
		if (diff > 20.0f)
			ScrollSign = 1;
		else if (diff < -20.0f)
			ScrollSign = -1;
	}

	return FReply::Handled();
}

FReply SRiffleScrollBox::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (GlobalSetting::UseTouchScreen())
		return FReply::Handled();

	RunScroll = false;
	MoveEnable = false;
	return FReply::Handled();
}

float SRiffleScrollBox::GetDiffOffset(const FPointerEvent& InEvent)
{
	if (GetOrientation() == EOrientation::Orient_Horizontal)
		return InEvent.GetScreenSpacePosition().X - Begin.X;
	else
		return InEvent.GetScreenSpacePosition().Y - Begin.Y;
}

FReply SRiffleScrollBox::OnTouchStarted(const FGeometry& MyGeometry, const FPointerEvent& InTouchEvent)
{
	RunScroll = false;
	MoveEnable = false;
	Begin = InTouchEvent.GetScreenSpacePosition();
	ScrollSign = 0;
	return FReply::Handled();
}

FReply SRiffleScrollBox::OnTouchMoved(const FGeometry& MyGeometry, const FPointerEvent& InTouchEvent)
{
	if(RunScroll)
		return FReply::Handled();

	float diff = GetDiffOffset(InTouchEvent);
	if (FMath::Abs(diff) > 20.0f)
	{
		MoveEnable = true;
		if (diff > 20.0f)
			ScrollSign = 1;
		else if (diff < -20.0f)
			ScrollSign = -1;
	}
	
	return FReply::Handled();
}

FReply SRiffleScrollBox::OnTouchEnded(const FGeometry& MyGeometry, const FPointerEvent& InTouchEvent)
{
	RunScroll = false;
	MoveEnable = false;
	return FReply::Handled();
}

TSharedRef<SWidget> UURiffleScrollBox::RebuildWidget()
{
	TSharedPtr<class SRiffleScrollBox> MyCustomScrollBox = SNew(SRiffleScrollBox)
		.Style(&WidgetStyle)
		.ScrollBarStyle(&WidgetBarStyle)
		.Orientation(Orientation)
		.ConsumeMouseWheel(ConsumeMouseWheel)
		.NavigationDestination(NavigationDestination)
		.NavigationScrollPadding(NavigationScrollPadding)
		.ScrollWhenFocusChanges(ScrollWhenFocusChanges)
		.AnimateWheelScrolling(bAnimateWheelScrolling)
		.WheelScrollMultiplier(WheelScrollMultiplier)
		.OnUserScrolled(BIND_UOBJECT_DELEGATE(FOnUserScrolled, SlateHandleUserScrolled));

	MyScrollBox = MyCustomScrollBox;
	ScrollBoxPtr = MyCustomScrollBox.Get();

	for (UPanelSlot* PanelSlot : Slots)
	{
		if (UScrollBoxSlot* TypedSlot = Cast<UScrollBoxSlot>(PanelSlot))
		{
			TypedSlot->Parent = this;
			TypedSlot->BuildSlot(MyScrollBox.ToSharedRef());
		}
	}

	return MyScrollBox.ToSharedRef();
}

bool UURiffleScrollBox::GetMoveEnable()
{
	if (MyScrollBox.IsValid())
		return ScrollBoxPtr->GetMoveEnable();
	else
		return false;
	return true;
}

bool UURiffleScrollBox::GetRunScroll()
{
	if (MyScrollBox.IsValid())
		return ScrollBoxPtr->GetRunScroll();
	else
		return false;
	return true;
}

void UURiffleScrollBox::SetRunScroll(bool IsScroll)
{
	if (MyScrollBox.IsValid())
		return ScrollBoxPtr->SetRunScroll(IsScroll);
}

int UURiffleScrollBox::GetScrollSign()
{
	if (MyScrollBox.IsValid())
		return ScrollBoxPtr->GetScrollSign();
	return 0;
}
