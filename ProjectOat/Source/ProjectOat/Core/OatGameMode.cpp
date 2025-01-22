// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/OatGameMode.h"
#include "Core/OatPlayerController.h"

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

	
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/ProjectOat.OatPlayerController_C"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}

}
 