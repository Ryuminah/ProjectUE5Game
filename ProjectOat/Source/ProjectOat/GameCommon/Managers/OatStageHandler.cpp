// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCommon/Managers/OatStageHandler.h"
#include "GameCommon/Managers/OatEventHandler.h"
#include "Shared/Enums.h"
#include "Core/OatGameInstance.h"

UOatStageHandler::UOatStageHandler()
{}

void UOatStageHandler::PostInitProperties()
{
	Super::PostInitProperties();
}

void UOatStageHandler::Initialize(UGameInstance* GameInstance)
{
	UOatGameInstance* OatGameInstance = Cast<UOatGameInstance>(GameInstance);
	if (!OatGameInstance)
	{
		ensure(TEXT("OatGameInstance is Null"));
		return;
	}

	OatGameInstance->GetEventHandler()->OnStageClearGoal.AddUObject(this, &UOatStageHandler::ReachedStageGoal);
	OatGameInstance->GetEventHandler()->OnStageClearDungeon.AddUObject(this, &UOatStageHandler::ClearDungeon);
	OatGameInstance->GetEventHandler()->OnGameOver.AddUObject(this, &UOatStageHandler::GameOver);
}

// 여기서부터는 인게임 실행 뒤 호출되는 로직이라서, GetWorld()도 안전하다
void UOatStageHandler::ReachedStageGoal()
{
	// 골에 도달했을 때
	// 현재 스테이지 정보 갱신
	// 다음 스테이지로 넘겨준다

	// 지금은 게임 종료

	UOatGameInstance* OatGameInstance = Cast<UOatGameInstance>(GetWorld()->GetGameInstance());
	if (OatGameInstance)
	{
		OatGameInstance->QuitGame();
	}
}

void UOatStageHandler::ClearDungeon()
{
	// 정산 보상
	// 1.대시 
	// 2.일섬
	// 3.발차기
}

void UOatStageHandler::GameOver()
{
	// 로비 씬으로 돌려보내기
	// 페이드 아웃.. 
	// 씬에 따라 결과창 ?
}

void UOatStageHandler::ExitLevel()
{}

void UOatStageHandler::EnterLevel()
{}
