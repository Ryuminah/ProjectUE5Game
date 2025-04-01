// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Stage/OatWallBattleField.h"
#include "Shared/Enums.h"
#include "Core/Managers/OatEventHandler.h"
#include "Core/OatGameInstance.h"

#include "Components/BoxComponent.h"

// Sets default values
AOatWallBattleField::AOatWallBattleField()
{
	WallBattlefield = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallBattlefield"));
	WallCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("WallCollision"));

	SetRootComponent(WallBattlefield);
	WallBattlefield->SetWorldScale3D(FVector(5.0f,5.0f,1.f));
	WallBattlefield->SetRelativeRotation(FRotator(0.0f,90.0f,90.f));

	WallCollision->SetupAttachment(WallBattlefield);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WallRef(TEXT("/Script/Engine.StaticMesh'/Game/ProjectOat/Arts/Backgrounds/Environments/Wall/SM_Wall_BattleField.SM_Wall_BattleField'"));
	if (WallRef.Object)
	{
		WallBattlefield->SetStaticMesh(WallRef.Object);
	}
	
	WallCollision->SetBoxExtent(FVector(200.f, 200.f, 40.f));
	WallCollision->SetRelativeLocation(FVector(100.f, 0.f, 0.f));

	// 카메라를 제외하고는 움직이는 건 전부 막음
	WallCollision->SetCollisionProfileName("BlockAllDynamic");
	WallCollision->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

}

void AOatWallBattleField::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// 생성자로 바궈도 될듯?
	UOatGameInstance* OatGameInstance = Cast<UOatGameInstance>(GetGameInstance());
	if (!OatGameInstance)
	{
		return;
	}
	
	OatGameInstance->GetEventHandler()->AddStageSectionChangedDelegate(this, static_cast<void(UObject::*)(int, EStageSectionState)>(&AOatWallBattleField::BattleStateChanged));

	// 처음에 숨김
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

void AOatWallBattleField::BattleStateChanged(int CurrentSectionId, EStageSectionState CurrentState)
{
	// 전투 시작일 경우
	if (CurrentSectionId != SectionId)
	{
		return;
	}
	
	if (CurrentState == EStageSectionState::READYBATTLE)
	{
		SetActorHiddenInGame(false);
		SetActorEnableCollision(true);
	}	

	// 전투 종료의 경우 해당 섹션의 BattleField는 삭제해버림
	else if (CurrentState == EStageSectionState::ENDBATTLE)
	{
		Destroy();
	}
}

