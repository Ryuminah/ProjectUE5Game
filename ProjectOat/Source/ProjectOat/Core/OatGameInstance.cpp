// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/OatGameInstance.h"
#include "GameCommon//Managers/OatStageHandler.h"
#include "GameCommon//Managers/OatEventHandler.h"

UOatGameInstance::UOatGameInstance()
{

}

void UOatGameInstance::Init()
{
	Super::Init();

	EventHandler = NewObject<UOatEventHandler>(this);
	StageHandler = NewObject<UOatStageHandler>(this);
}

// UObject의 모든 속성이 초기화 된 이후, 에디터에서 Spawn될 가능성이 있어도 안정적
void UOatGameInstance::PostInitProperties()
{
	Super::PostInitProperties();

}
