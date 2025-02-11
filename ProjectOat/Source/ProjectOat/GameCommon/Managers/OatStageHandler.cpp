// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCommon/Managers/OatStageHandler.h"
#include "GameCommon/Managers/OatEventHandler.h"

#include "Core/OatGameInstance.h"

UOatStageHandler::UOatStageHandler()
{}

void UOatStageHandler::PostInitProperties()
{
	Super::PostInitProperties();

	UOatGameInstance* OatGameInstance = Cast<UOatGameInstance>(GetWorld()->GetGameInstance());
	if (!OatGameInstance)
	{
		ensure(TEXT("OatGameInstance is Null"));
		return;
	}

	OatGameInstance->GetEventHandler()->OnStageClearGoal.AddUObject(this, &UOatStageHandler::ReachedStageGoal);
}

void UOatStageHandler::ReachedStageGoal()
{
	// 골에 도달했을 때
	// 현재 스테이지 정보 갱신
	// 다음 스테이지로 넘겨준다

	// 지금은 게임 종료
}
