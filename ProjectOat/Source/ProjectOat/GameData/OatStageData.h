// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Shared/Enums.h"
#include "OatStageData.generated.h"


/**
 * 인게임 중에 필요한 유저 스테이지 정보
 */
USTRUCT()
struct FOatStageData
{
	GENERATED_BODY()
public:
	// 영구 저장 되어야할 데이터
	// UPROPERTY()
	// int HighestStage;

	// 몇 스테이지인지?
	UPROPERTY()
	int Stage;

	// 현재 섹션 Id
	UPROPERTY()
	int SectionId;

	// 섹션 내에 몇 웨이브 째 인지?
	UPROPERTY()
	int SectionWave;
	
	// 섹션 내에 죽인 적 계산
	UPROPERTY()
	int KillCount;
	
	UPROPERTY()
	EStageSectionState StageSectionState;

	/*GetStageDataTable*/
	
};
