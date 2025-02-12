// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/OatGameMode.h"
#include "Core/OatPlayerController.h"
#include "Core/OatGameInstance.h"
#include "GameCommon/Managers/OatEventHandler.h"
#include "GameCommon/Managers/OatStageHandler.h"

AOatGameMode::AOatGameMode()
{
	//DefaultPawnClass
	// Class�� UClass��ü(Ŭ���� ����)�� �������� �Լ�
	//static ConstructorHelpers::FClassFinder<APawn> ThirdPersonClassRef(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter_C"));
	//if (ThirdPersonClassRef.Class)
	//{
	//	DefaultPawnClass = ThirdPersonClassRef.Class;
	//}

	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Game/ProjectOat/InGame/Character/Roxy/BP_RoxyCharacter.BP_RoxyCharacter_C"));
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

// ���� ����ÿ��� ���� �̰� ȣ��
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

void AOatGameMode::OnPlayerDead()
{
	AOatPlayerController* OatPlayerController = Cast<AOatPlayerController>(GetWorld()->GetFirstPlayerController());

	if (OatPlayerController)
	{
		return;
	}

	OatPlayerController->GameOver();
}

 