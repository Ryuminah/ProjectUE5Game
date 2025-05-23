// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/OatGameMode.h"
#include "Core/OatPlayerController.h"
#include "Core/OatGameInstance.h"
#include "Core/Managers/OatEventHandler.h"
#include "Shared/OatUtilities.h"

AOatGameMode::AOatGameMode()
{
	//DefaultPawnClass
	// Class의 UClass객체(클래스 정보)를 가져오는 함수
	//static ConstructorHelpers::FClassFinder<APawn> ThirdPersonClassRef(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter_C"));
	//if (ThirdPersonClassRef.Class)
	//{
	//	DefaultPawnClass = ThirdPersonClassRef.Class;
	//}

	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Game/ProjectOat/InGame/Character/Oat/BP_OatCharacter.BP_OatCharacter_C"));
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	} 

	
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/ProjectOat.OatPlayerController"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}

	//
	CurrentScore = 0;
	bIsCleared = false;
}

// 게임 오버시에는 전부 이거 호출
void AOatGameMode::GameOver()
{
	UOatGameInstance* OatGameInstance = Cast<UOatGameInstance>(GetGameInstance());
	if (!OatGameInstance)
	{
		return;
	}

	OatGameInstance->GetEventHandler()->OnGameOver.Broadcast();
}

void AOatGameMode::GameClear()
{
	// 게임 클리어 시 
	UOatGameInstance* OatGameInstance = Cast<UOatGameInstance>(GetGameInstance());
	if (!OatGameInstance)
	{
		return;
	}

	ELevelType CurrentLevelType = OatGameInstance->GetCurrentLevel();
	switch (CurrentLevelType)
	{
	case ELevelType::LOBBY:
		break;
	case ELevelType::SELECT:
		break;
	case ELevelType::MAINSTAGE:
		OatGameInstance->GetEventHandler()->OnStageClearGoal.Broadcast();
		break;
	case ELevelType::DUNGEON:
		OatGameInstance->GetEventHandler()->OnStageClearDungeon.Broadcast();
		break;
	}
}

bool AOatGameMode::IsGameCleared()
{

	return false;
}

// 이거 이상함 PlayerController로 빼던가 해야함
void AOatGameMode::OnPlayerDead()
{
	AOatPlayerController* OatPlayerController = Cast<AOatPlayerController>(GetWorld()->GetFirstPlayerController());

	if (OatPlayerController)
	{
		return;
	}

	OatPlayerController->GameOver();
}

 