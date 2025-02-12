// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/OatGameInstance.h"
#include "GameCommon/Managers/OatStageHandler.h"
#include "GameCommon/Managers/OatEventHandler.h"
#include "GameCommon/Managers/OatHandlerInterface.h"
#include "Shared/Enums.h"


#include "Kismet/KismetSystemLibrary.h"

UOatGameInstance::UOatGameInstance()
{
	CurrentLevelType = ELevelType::SELECT;
}

void UOatGameInstance::Init()
{
	Super::Init();

	TArray<IOatHandlerInterface*> HandlersArray;
	EventHandler = NewObject<UOatEventHandler>();
	HandlersArray.Add(EventHandler);

	StageHandler = NewObject<UOatStageHandler>();
	HandlersArray.Add(StageHandler);

	// �ʱ�ȭ ȣ��
	for (auto Handler : HandlersArray)
	{
		Handler->Initialize(this);
	}

}

// UObject�� ��� �Ӽ��� �ʱ�ȭ �� ����, �����Ϳ��� Spawn�� ���ɼ��� �־ ������
void UOatGameInstance::PostInitProperties()
{
	Super::PostInitProperties();

}

void UOatGameInstance::SetupGameEventBindings()
{


}


void UOatGameInstance::TravelLevel(ELevelType LevelType)
{
	// ���� �̵�
	if (GetCurrentLevel() == LevelType)
	{
		return;
	}

	// ���� ���� ����
	GetStageHandler()->ExitLevel();
	
	// �̵� ��
	CurrentLevelType = LevelType;

	switch (CurrentLevelType)
	{
	case ELevelType::LOBBY:
		break;
	case ELevelType::SELECT:
		break;
	case ELevelType::MAINSTAGE:
		break;
	case ELevelType::DUNGEON:
		break;
	default:
		break;
	}

	GetStageHandler()->EnterLevel();
}

void UOatGameInstance::QuitGame()
{
	GetEventHandler()->OnQuitGame.Broadcast();

	// �켱�� ���⼭ �����ϴµ�..
	UKismetSystemLibrary::QuitGame(this, GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}
