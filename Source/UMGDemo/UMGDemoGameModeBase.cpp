// Copyright Epic Games, Inc. All Rights Reserved.


#include "UMGDemoGameModeBase.h"
#include "UI_Main.h"
#include "Kismet/GameplayStatics.h"

#pragma optimize( "", off )

AUMGDemoGameModeBase::AUMGDemoGameModeBase()
{
	UE_LOG(LogTemp, Warning, TEXT("%s (%d)"), *FString(__FUNCTION__), __LINE__);

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_WIDGET(TEXT("/Game/UI/BP_UI_Main.BP_UI_Main_C"));
	UI_MainClass = UI_WIDGET.Class;
}

void AUMGDemoGameModeBase::StartPlay()
{
	if (GetWorld() == nullptr)
		return;

	if(UI_MainClass)
	{
		UUserWidget* UserWidget = CreateWidget<UUserWidget>(GetWorld(), UI_MainClass);
		UI_Main = Cast<UUI_Main>(UserWidget);
		UI_Main->AddToViewport();
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	PlayerController->SetInputMode(FInputModeUIOnly());
	PlayerController->SetShowMouseCursor(true);
}

#pragma optimize( "", on )