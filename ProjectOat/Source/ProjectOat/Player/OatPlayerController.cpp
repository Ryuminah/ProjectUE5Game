// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/OatPlayerController.h"

void AOatPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// ���� ���� �� ���콺�� ����Ʈ ���ο� ����
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}
