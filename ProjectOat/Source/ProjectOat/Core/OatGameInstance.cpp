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

// UObject�� ��� �Ӽ��� �ʱ�ȭ �� ����, �����Ϳ��� Spawn�� ���ɼ��� �־ ������
void UOatGameInstance::PostInitProperties()
{
	Super::PostInitProperties();

}
