// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/OatGameInstance.h"
#include "Core/Managers/OatStageHandler.h"
#include "Core/Managers/OatEventHandler.h"
#include "Core/Managers/OatHandlerInterface.h"
#include "Shared/Enums.h"


#include "Kismet/KismetSystemLibrary.h"

UOatGameInstance::UOatGameInstance()
{
	CurrentLevelType = ELevelType::MAINSTAGE;
}

void UOatGameInstance::Init()
{
	Super::Init();

	UWorld* World = GetWorld();
	if (!World)
	{
		ensure("UOatGameInstance::Init() if(!World)");
	}

	TArray<IOatHandlerInterface*> HandlersArray;
	EventHandler = World->SpawnActor<AOatEventHandler>();
	HandlersArray.Add(EventHandler);

	StageHandler = World->SpawnActor<AOatStageHandler>();
	HandlersArray.Add(StageHandler);

	// 초기화 호출
	for (auto Handler : HandlersArray)
	{
		Handler->Initialize(this);
	}

	SetupGameEventBindings();

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
	GetStageHandler()->ExitLevel(LevelType);
	
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

	GetStageHandler()->EnterLevel(LevelType);
}

void UOatGameInstance::QuitGame()
{
	// 종료
	GetEventHandler()->OnQuitGame.Broadcast();

	// 우선은 여기서 종료하는데...
	UKismetSystemLibrary::QuitGame(this, GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
	UE_LOG(LogTemp, Log, TEXT("UOatGameInstance::QuitGame()"));
}
