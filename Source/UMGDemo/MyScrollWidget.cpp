// Fill out your copyright notice in the Description page of Project Settings.


#include "MyScrollWidget.h"
#include "ScrollItem.h"

#pragma optimize( "", off )

UMyScrollWidget::UMyScrollWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ScrollType = EScrollType::Card;
}

bool UMyScrollWidget::Initialize()
{
	if (Super::Initialize() == false)
		return false;

#if  1
	AddItem("Start", { 0.8f, 0.8f });
	for (int n = 0; n < 7; ++n)
	{
		FString Name = FString::Printf(TEXT("%04d"), n);

		FVector2D Scale = { 0.8f, 0.8f };
		AddItem(Name, Scale);
	}
	AddItem("End", { 0.8f, 0.8f });

#else 

	FVector2D Scale0 = { 0.8f, 0.8f };
	AddItem(TEXT("0001"), Scale0);

	FVector2D Scale1 = { 1.2f, 1.2f };
	AddItem(TEXT("0001"), Scale1);

	FVector2D Scale2 = { 0.8f, 0.8f };
	AddItem(TEXT("0001"), Scale2);
#endif

	return true;
}

void UMyScrollWidget::AddItem(FString str, const FVector2D& Scale)
{
	FStringClassReference SBWidgetRef(TEXT("/Game/UI/BP_ListItem.BP_ListItem_C"));
	if (UClass* WidgetClass = SBWidgetRef.TryLoadClass<UUserWidget>())
	{
		UUserWidget* UserWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
		UScrollItem* ListItem = Cast<UScrollItem>(UserWidget);
		if (ListItem)
			ListItem->SetItem(str);

		UTouchScrollWidget::AddChild(UserWidget);
		UserWidget->SetRenderScale(Scale);
	}
}


void UMyScrollWidget::ClickItem(UUserWidget* Widget)
{
	Super::ClickItem(Widget);

	if (SelectItemPtr.IsValid())
	{
		UScrollItem* item = Cast< UScrollItem>(SelectItemPtr.Get());
		if (item)
		{
			FVector2D Scale = FVector2D(3.0f, 3.0f);
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, item->Data_GetText(), true, Scale);
		}
	}
}

#pragma optimize( "", on )