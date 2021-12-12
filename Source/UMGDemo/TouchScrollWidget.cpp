// Fill out your copyright notice in the Description page of Project Settings.


#include "TouchScrollWidget.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/ScrollBox.h"
#include "TouchToggleButton.h"
#include "Blueprint/WidgetTree.h"
#include "URiffleScrollBox.h"

#define SCROLL_INTOVIEW  1

#pragma optimize( "", off ) 

UTouchScrollWidget::UTouchScrollWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ButtonUpPtr = nullptr;
	ButtonDownPtr = nullptr;
	ScrollBoxPtr = nullptr;
	SelectItemPtr = nullptr;
	ScrollType = EScrollType::List;
	LastScrollOffset = 0;
}

void UTouchScrollWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!IsBeginTick.IsSet())
	{
		IsBeginTick = true;

		if (ScrollType == EScrollType::Card)
		{
			BuildCard();
			StartScrollOffset();
			InitReScaleCardItem();
		}
	}
	else
	{
		UScrollBox* ScrollBox = ScrollBoxPtr.Get();
		UURiffleScrollBox* RiffleScrollBox = Cast<UURiffleScrollBox>(ScrollBox);
		if (RiffleScrollBox)
		{
			if (RiffleScrollBox->GetMoveEnable() && !RiffleScrollBox->GetRunScroll())
			{
				RiffleScrollBox->SetRunScroll(true);
				float ScrollSign = (float)RiffleScrollBox->GetScrollSign();
				if (ScrollSign >= 1.0f)
					OnScrollDown();
				else if (ScrollSign <= -1.0f)
					OnScrollUp();
			}
		}
	}
}

bool UTouchScrollWidget::Initialize()
{
	if(Super::Initialize() == false)
		return false;

	BindPointer();
	AddEvent();
	return true;
}

void UTouchScrollWidget::BuildCard()
{
	if (!ScrollBoxPtr.IsValid())
		return;

	UScrollBox* ScrollBox = ScrollBoxPtr.Get();
	float ScrollOffset = ScrollBox->GetScrollOffset();
	int count = ScrollBox->GetChildrenCount();
	if (count == 0)
		return;

	UWidget* Child = ScrollBox->GetChildAt(1); 
	if(count > 1)
		Child = ScrollBox->GetChildAt(2); //0번 인덱스 아이템은비쥬얼 더미(보이지 않는 아이템)이다.

	UUserWidget* UserWidget = Cast<UUserWidget>(Child);
	if (UserWidget)
	{
		ClickItem(UserWidget);
		CheckItem(UserWidget, ECheckBoxState::Checked);
	}

	UWidget* StartDummyChild = ScrollBox->GetChildAt(0);
	StartDummyChild->SetVisibility(ESlateVisibility::Hidden);

	UWidget* EndDummyChild = ScrollBox->GetChildAt(count - 1);
	EndDummyChild->SetVisibility(ESlateVisibility::Hidden);
}

void UTouchScrollWidget::InitReScaleCardItem()
{
	if (ScrollType != EScrollType::Card)
		return;

	if (!ScrollBoxPtr.IsValid())
		return;

	UScrollBox* ScrollBox = ScrollBoxPtr.Get();
	if (ScrollBox->GetChildrenCount() <= 0)
		return;
	
	for (int n = 0; n < ScrollBox->GetChildrenCount(); ++n)
	{
		UWidget* Child = ScrollBox->GetChildAt(n);
		if (Child == SelectItemPtr)
			Child->SetRenderScale({ 1.2f, 1.2f });
		else
			Child->SetRenderScale({ 0.8f, 0.8f });
	}

	LastScrollOffset = ScrollBox->GetScrollOffset();
	UE_LOG(LogTemp, Warning, TEXT("%s (%d)"), *FString(__FUNCTION__), __LINE__);
}

void UTouchScrollWidget::ReSelectScaleCardItem()
{
	if (ScrollType != EScrollType::Card)
		return;

	if (!ScrollBoxPtr.IsValid())
		return;

	UScrollBox* ScrollBox = ScrollBoxPtr.Get();
	if (ScrollBox->GetChildrenCount() <= 1)
		return;

	UWidget* Child = ScrollBox->GetChildAt(0);
	FVector2D ChildSize = Child->GetDesiredSize();

	float ScrollOffset = ScrollBox->GetScrollOffset();
	EOrientation Orientation = ScrollBox->Orientation;

	float CSize = ChildSize.Y;
	if (Orientation == EOrientation::Orient_Horizontal)
		CSize = ChildSize.X;

	int index = int(ScrollOffset / CSize) + 1;

	UWidget* SelectChild = ScrollBox->GetChildAt(index);
	UUserWidget* UserWidget = Cast<UUserWidget>(SelectChild);

	if (UserWidget)
	{
		UUserWidget* oldItem = SelectItemPtr.Get();
		if (oldItem)
		{
			CheckItem(oldItem, ECheckBoxState::Unchecked);
			oldItem->SetRenderScale({ 0.8f, 0.8f });
		}

		SelectItemPtr = UserWidget;
		CheckItem(UserWidget, ECheckBoxState::Checked);
		UserWidget->SetRenderScale({1.2f, 1.2f});
	}

	LastScrollOffset = ScrollBox->GetScrollOffset();
}


void UTouchScrollWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	const auto World = GetWorld();
	if (nullptr != World && World->IsPlayInEditor())
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT(" SynchronizeProperties"));
		BindPointer();
		AddEvent();
	}
}

void UTouchScrollWidget::BindPointer()
{
	if (!ButtonDownName.IsEmpty())
	{
		UButton* button = Cast<UButton>(GetWidgetFromName(*ButtonDownName));
		if (button)
			ButtonDownPtr = button;
	}

	if (!ButtonUpName.IsEmpty())
	{
		UButton* button = Cast<UButton>(GetWidgetFromName(*ButtonUpName));
		if (button)
			ButtonUpPtr = button;
	}

	if (!ScrollBoxName.IsEmpty())
	{
		UScrollBox* ScrollBox = Cast<UScrollBox>(GetWidgetFromName(*ScrollBoxName));
		if (ScrollBox)
			ScrollBoxPtr = ScrollBox;
	}
}

void UTouchScrollWidget::AddEvent()
{
	if (ButtonDownPtr.IsValid())
	{
		UButton* ButtonDown = ButtonDownPtr.Get();
		ButtonDown->OnClicked.RemoveDynamic(this, &UTouchScrollWidget::OnScrollUp);
		ButtonDown->OnClicked.AddDynamic(this, &UTouchScrollWidget::OnScrollUp);

		UButton* ButtonUp = ButtonUpPtr.Get();
		ButtonUp->OnClicked.RemoveDynamic(this, &UTouchScrollWidget::OnScrollDown);
		ButtonUp->OnClicked.AddDynamic(this, &UTouchScrollWidget::OnScrollDown);

		if (ScrollBoxPtr.IsValid())
		{
			UScrollBox* ScrollBox = ScrollBoxPtr.Get();
			ScrollBox->OnUserScrolled.RemoveDynamic(this, &UTouchScrollWidget::OnHandleUserScrolled);
			ScrollBox->OnUserScrolled.AddDynamic(this, &UTouchScrollWidget::OnHandleUserScrolled);
		}
	}
}

void UTouchScrollWidget::OnHandleUserScrolled(float CurrentOffset)
{
	UE_LOG(LogTemp, Warning, TEXT("%s (%d) -> %d"), *FString(__FUNCTION__), __LINE__, CurrentOffset);
	ReSelectScaleCardItem();
}

float UTouchScrollWidget::GetChildSize()
{
	if (!ScrollBoxPtr.IsValid())
		return 0.0f;

	UScrollBox* ScrollBox = ScrollBoxPtr.Get();
	int count = ScrollBox->GetChildrenCount();
	if (count == 0)
		return 0.0f;

	UWidget* Child = ScrollBox->GetChildAt(0);
	FVector2D ChildSize = Child->GetDesiredSize();
	if (ScrollBox->Orientation == EOrientation::Orient_Horizontal)
		return ChildSize.X;
	else
		return ChildSize.Y;
}

float UTouchScrollWidget::GetLocalSize()
{
	if (!ScrollBoxPtr.IsValid())
		return 0.0f;

	UScrollBox* ScrollBox = ScrollBoxPtr.Get();
	const FGeometry& Geometry = ScrollBox->GetTickSpaceGeometry();
	FVector2D ViewSize = Geometry.GetLocalSize();

	if (ScrollBox->Orientation == EOrientation::Orient_Horizontal)
		return ViewSize.X;
	else
		return ViewSize.Y;
}

void UTouchScrollWidget::StartScrollOffset()
{
	if (!ScrollBoxPtr.IsValid())
		return;

	UScrollBox* ScrollBox = ScrollBoxPtr.Get();
	float ScrollOffset = ScrollBox->GetScrollOffset();
	int count = ScrollBox->GetChildrenCount();
	if (count == 0)
		return;

	float ChildSize = GetChildSize();

	//최초에는 인덱스 2번부터 선택 되어 있다.
	BeginOffset = ChildSize * 0.5f;

	if(count == 1)
		ScrollBox->SetScrollOffset(BeginOffset);
	else
		ScrollBox->SetScrollOffset(BeginOffset + ChildSize);
}

void UTouchScrollWidget::OnScrollUp()
{
	if (!ScrollBoxPtr.IsValid())
		return;

	UScrollBox* ScrollBox = ScrollBoxPtr.Get();
	float ScrollOffset = ScrollBox->GetScrollOffset();
	int count = ScrollBox->GetChildrenCount();
	if (count == 0)
		return;

	float ChildSize = GetChildSize();

	float ViewSize = GetLocalSize();
	
#ifdef SCROLL_INTOVIEW

	float MaxScrollSize = ChildSize * count + BeginOffset;
	if (ScrollOffset + ChildSize < MaxScrollSize - ViewSize - BeginOffset)
	{
		UUserWidget* SelectItem = GetSelectItem();
		if (SelectItem == nullptr)
			return;
		int index = ScrollBox->GetChildIndex(SelectItem);

		UUserWidget* ChildWidget = Cast<UUserWidget>(ScrollBox->GetChildAt(index + 1));
		if (ChildWidget)
		{
			ScrollBox->ScrollWidgetIntoView(ChildWidget, true, EDescendantScrollDestination::Center, 0.0f);
			ReSelectScaleCardItem();
		}
	}
#else
#if  0
	//BeginOffset가 0일때 사용 하던것
	float MaxScrollSize = ChildSize * count;
	if (ScrollOffset + ChildSize < MaxScrollSize - ViewSize)
	{
		ScrollBox->SetScrollOffset(ScrollOffset + ChildSize);
	}
	else
		ScrollBox->SetScrollOffset(MaxScrollSize - ViewSize);

	ReSelectScaleCardItem();
#else
	float MaxScrollSize = ChildSize * count + BeginOffset;
	if (ScrollOffset + ChildSize < MaxScrollSize - ViewSize - BeginOffset)
	{
		ScrollBox->SetScrollOffset(ScrollOffset + ChildSize);
		ReSelectScaleCardItem();
	}
#endif
#endif
}

void UTouchScrollWidget::OnScrollDown()
{
	if (!ScrollBoxPtr.IsValid())
		return;
	UScrollBox* ScrollBox = ScrollBoxPtr.Get();

	float ScrollOffset = ScrollBox->GetScrollOffset();
	int count = ScrollBox->GetChildrenCount();
	if (count == 0)
		return;

	float ChildSize = GetChildSize();

#ifdef SCROLL_INTOVIEW

	if (ScrollOffset - ChildSize < 0)
		return;
	else
	{
		UUserWidget* SelectItem = GetSelectItem();
		if (SelectItem == nullptr)
			return;
		int index = ScrollBox->GetChildIndex(SelectItem);

		UUserWidget* ChildWidget = Cast<UUserWidget>(ScrollBox->GetChildAt(index - 1));
		if (ChildWidget)
		{
			ScrollBox->ScrollWidgetIntoView(ChildWidget, true, EDescendantScrollDestination::Center, 0.0f);
			ReSelectScaleCardItem();
		}

	}
	ReSelectScaleCardItem();

#else
	
#if  0
	//BeginOffset가 0일때 사용 하던것
	if (ScrollOffset - ChildSize < 0)
		ScrollBox->SetScrollOffset(0);
	else
		ScrollBox->SetScrollOffset(ScrollOffset - ChildSize);

	ReSelectScaleCardItem();
#else
	if (ScrollOffset - ChildSize < 0)
		return;
	else
		ScrollBox->SetScrollOffset(ScrollOffset - ChildSize);

	ReSelectScaleCardItem();
#endif
#endif
}

void UTouchScrollWidget::CheckItem(UUserWidget* Widget, ECheckBoxState InCheckedState)
{
	TArray<UWidget*> WidgetList;
	Widget->WidgetTree->GetAllWidgets(WidgetList);
	if (WidgetList.Num() == 0)
		return;

	for (int n = 0; n < WidgetList.Num(); ++n)
	{
		UCheckBox* CheckBox = Cast<UCheckBox>(WidgetList[n]);
		if (CheckBox)
		{
			CheckBox->SetCheckedState(InCheckedState);
			break;
		}
	}
}

void UTouchScrollWidget::CheckItem(int index, ECheckBoxState InCheckedState)
{
	if (!ScrollBoxPtr.IsValid())
		return;
	UScrollBox* ScrollBox = ScrollBoxPtr.Get();
	int count = ScrollBox->GetChildrenCount();
	if (count == 0)
		return;

	UUserWidget* UserWidget = Cast<UUserWidget>(ScrollBox->GetChildAt(0));
	if (UserWidget)
		CheckItem(UserWidget, InCheckedState);
}

void UTouchScrollWidget::BindTouchScrollEvent(UUserWidget* UserWidget)
{
	TArray<UWidget*> WidgetList;
	UserWidget->WidgetTree->GetAllWidgets(WidgetList);
	if (WidgetList.Num() == 0)
		return;

	for (int n = 0; n < WidgetList.Num(); ++n)
	{
		UTouchToggleButton* touchToggleButton = Cast<UTouchToggleButton>(WidgetList[n]);
		if (touchToggleButton)
		{
			touchToggleButton->OnClickTouchButton.AddDynamic(this, &UTouchScrollWidget::OnClickListItem);
			break;
		}
	}
}

UUserWidget* GetUserWidget(UWidget* Widget)
{
	UWidgetTree* WidgetTree = Cast<UWidgetTree>(Widget->GetOuter());
	if (WidgetTree == nullptr)
		return nullptr;

	UUserWidget* UserWidget = Cast<UUserWidget>(WidgetTree->GetOuter());
	return UserWidget;
}

void UTouchScrollWidget::OnClickListItem(UWidget* Widget)
{
	if (!ScrollBoxPtr.IsValid())
		return;
	UScrollBox* ScrollBox = ScrollBoxPtr.Get();

	UWidget* Parent = Widget->GetParent();
	UUserWidget* UserWidget = nullptr;
	   
	TSubclassOf<UUserWidget> NewWidgetClass;

	while (Parent)
	{
		UserWidget = GetUserWidget(Widget);
		if(UserWidget)
			break;;
		Parent = Parent->GetParent();
	}

	if (UserWidget == nullptr)
		return;

	ClickItem(UserWidget);
}

UUserWidget* UTouchScrollWidget::GetSelectItem()
{
	if (!SelectItemPtr.IsValid())
		return nullptr;

	return SelectItemPtr.Get();
}

void UTouchScrollWidget::ClickItem(UUserWidget* Widget)
{
	UE_LOG(LogTemp, Warning, TEXT("Click Scroll Item"));
	
	UUserWidget* OldItem = GetSelectItem();
	if (OldItem)
	{
		if(OldItem == Widget)
			CheckItem(OldItem, ECheckBoxState::Checked);
		else
			CheckItem(OldItem, ECheckBoxState::Unchecked);
	}

	SelectItemPtr = Widget;
}

void UTouchScrollWidget::AddChild(UUserWidget* UserWidget)
{
	if (!ScrollBoxPtr.IsValid())
		return;

	BindTouchScrollEvent(UserWidget);

	UScrollBox* ScrollBox = ScrollBoxPtr.Get();
	ScrollBox->AddChild(UserWidget);
}

#pragma optimize( "", on ) 


/*
UTouchScrollWidget 원리 설명

CheckBox의 Check는 TouchToggleButton에서 담당
CheckBox의 UnCheck는 UTouchScrollWidget에서 담당
*/

/*
#1
class UI_TEST_API UTouchScrollWidget : public UUserWidget
    ....
    UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
    TWeakObjectPtr<class UButton> ButtonUpPtr;

#2
이건 안됨. 왜냐하면 UE4 Widget 에디터에서 리스트가 하나도 안보인다.
아래와 같이 해야 배치된 버튼 리스트가 보인다.

	UPROPERTY(EditInstanceOnly, meta = (AllowPrivateAccess = "true"))
	class UButton* ButtonUpPtr;

	//UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	//class UButton* ButtonUpPtr;

	UPROPERTY()
	class UScrollBox* ScrollBoxPtr;

#3
TWeakObjectPtr 사용법
TWeakObjectPtr<AActor> MyWeakActor;
MyWeakActor = MyActor;

TWeakObjectPtr::IsValid()

AActor* Actor = MyWeakActor.Get();

https://dawnarc.com/2018/07/ue4-tsharedptr-tweakobjectptr-and-tuniqueptr/


#4 
상속한 윗드젯이 최상위일때는 다른 컨트롤의 포인터를 가질수 있다.
하지만 최상위가 아닐때는 블루프린트 컴파일을 하면 다른 컨트롤의 포인터를 가질수 없다.
하지만 컨트롤은 다른 컨트롤의 포인터를 가질수 있다...... 
결론 규칙이 있는건가...내가 UPROPERTY 설정을 잘못한건가...
연구나 자료를 찾을 필요가 있음


#5
GetUserWidget 함수에서 Outer --> WidgetTree --> UUserWidget 구하는 방법 


#6
위젯의 크기를 구하기
FVector2D UWidget::GetDesiredSize()

#7 클릭하여 데이타 출력
void UScrollView::ClickItem(UUserWidget* Widget)

#8 AddOnScreenDebugMessage 폰트 크기 조정
FVector2D Scale = FVector2D(3.0f, 3.0f);
GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, item->Data_GetText(), true, Scale);

아래에서 참조했음 ... 캔바스에서 문자열 출력하는 방법
https://qiita.com/unknown_ds/items/759edd4c8b93ad1977cc

#9 
NativeOnTouchMoved등의 터치처리시 다음 라인이 빌드 안되면 어떻게 하지?
return FReply::Handled();

XXX.Build.cs에서Slate, SlateCore를 추가한다.
PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

*/

