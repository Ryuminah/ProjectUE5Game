// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/OatPlayerController.h"

void AOatPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 게임 시작 시 마우스가 뷰포트 내부에 적용
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}
