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

	// 초기화 호출
	for (auto Handler : HandlersArray)
	{
		Handler->Initialize(this);
	}

}

// UObject의 모든 속성이 초기화 된 이후, 에디터에서 Spawn될 가능성이 있어도 안정적
void UOatGameInstance::PostInitProperties()
{
	Super::PostInitProperties();

}

void UOatGameInstance::SetupGameEventBindings()
{


}


void UOatGameInstance::TravelLevel(ELevelType LevelType)
{
	// 레벨 이동
	if (GetCurrentLevel() == LevelType)
	{
		return;
	}

	// 현재 레벨 종료
	GetStageHandler()->ExitLevel();
	
	// 이동 후
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

	// 우선은 여기서 종료하는데..
	UKismetSystemLibrary::QuitGame(this, GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}
