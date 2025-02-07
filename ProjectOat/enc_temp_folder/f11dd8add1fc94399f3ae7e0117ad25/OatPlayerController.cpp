// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/OatPlayerController.h"
#include "InGame/UI/OatHUDWidget.h"

AOatPlayerController::AOatPlayerController()
{
	static ConstructorHelpers::FClassFinder<UOatHUDWidget> OatHUDWidgetRef(TEXT("/Game/ProjectOat/InGame/Widget/WBP_OatHUD.WBP_OatHUD_C"));
	if (OatHUDWidgetRef.Class)
	{
		OatHUDWidgetClass = OatHUDWidgetRef.Class;
	}
}

void AOatPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// ���� ���� �� ���콺�� ����Ʈ ���ο� ����
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

	OatHUDWidget = CreateWidget<UOatHUDWidget>(this, OatHUDWidgetClass);
	if (OatHUDWidget)
	{
		OatHUDWidget->AddToViewport();
	}
}
