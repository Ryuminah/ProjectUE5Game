// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/OatGameMode.h"
#include "Player/OatPlayerController.h"

AOatGameMode::AOatGameMode()
{
	//DefaultPawnClass
	// Class의 UClass객체(클래스 정보)를 가져오는 함수
	static ConstructorHelpers::FClassFinder<APawn> ThirdPersonClassRef(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter_C"));
	if (ThirdPersonClassRef.Class)
	{
		DefaultPawnClass = ThirdPersonClassRef.Class;
	}

	
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/ProjectOat.OatPlayerController"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}

}
 