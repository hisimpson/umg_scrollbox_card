// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "UObject/WeakObjectPtrTemplates.h"
#include "TouchScrollWidget.generated.h"

UENUM(BlueprintType)
enum class EScrollType : uint8
{
	List UMETA(DisplayName = "List"),
	Card UMETA(DisplayName = "Card")
};


/**
 * 
 */
UCLASS()
class UMGDEMO_API UTouchScrollWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UTouchScrollWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	class UUserWidget* GetSelectItem();
	void CheckItem(int index, ECheckBoxState InCheckedState);
	void CheckItem(UUserWidget* Widget, ECheckBoxState InCheckedState);

protected:
	virtual bool Initialize() override;
	virtual void SynchronizeProperties() override;

	//아이템이 클릭 될때, UTouchScrollWidget을 상속하고, 여기서 클릭 이벤트를 처리한다.
	virtual void ClickItem(UUserWidget* UserWidget);

	void BuildCard();
	void InitReScaleCardItem();
	void ReSelectScaleCardItem();
	float GetChildSize();
	float GetLocalSize();

	UFUNCTION()
	void OnScrollUp();
	UFUNCTION()
	void OnScrollDown();

	void BindPointer();
	void AddEvent();
	void AddChild(UUserWidget* UserWidget);
	void StartScrollOffset();

	void BindTouchScrollEvent(UUserWidget* UserWidget);
	
	UFUNCTION()
	void OnClickListItem(UWidget* Widget);

	UFUNCTION()
	void OnHandleUserScrolled(float CurrentOffset);

protected:
	TWeakObjectPtr<class UButton> ButtonUpPtr;
	
	TWeakObjectPtr<class UButton> ButtonDownPtr;

	TWeakObjectPtr<class UScrollBox> ScrollBoxPtr;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FString ButtonDownName;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FString ButtonUpName;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FString ScrollBoxName;

	TWeakObjectPtr<class UUserWidget> SelectItemPtr;

	TOptional<bool> IsBeginTick;

	EScrollType ScrollType;

	int LastScrollOffset;

	float BeginOffset = 0.0f;
};
