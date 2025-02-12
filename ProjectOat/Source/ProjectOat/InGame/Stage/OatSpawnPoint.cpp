// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Stage/OatSpawnPoint.h"
#include "Components/BoxComponent.h"


void AOatSpawnPoint::BeginDestroy()
{
	Super::BeginDestroy();


	UE_LOG(LogTemp, Warning, TEXT("BeginDestroy()"));
}

void AOatSpawnPoint::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	//switch (EndPlayReason)
	//{
	//case EEndPlayReason::Destroyed:
	//	UE_LOG(LogTemp, L, TEXT("EndPlay() - 액터가 Destroy()로 삭제됨"));
	//	break;
	//case EEndPlayReason::LevelTransition:
	//	UE_LOG(LogTemp, Warning, TEXT("EndPlay() - 레벨 전환으로 인해 삭제됨"));
	//	break;
	//case EEndPlayReason::EndPlayInEditor:
	//	UE_LOG(LogTemp, Warning, TEXT("EndPlay() - 에디터 종료로 인해 삭제됨"));
	//	break;
	//case EEndPlayReason::Quit:
	//	UE_LOG(LogTemp, Warning, TEXT("EndPlay() - 게임 종료로 인해 삭제됨"));
	//	break;
	//default:break;
	//}
}

// Sets default values
AOatSpawnPoint::AOatSpawnPoint()
{
	SpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnBox"));
	SetRootComponent(SpawnBox);

	SpawnBox->SetBoxExtent(FVector(1.f, 1.f, 1.f));
	SpawnBox->SetCollisionProfileName(TEXT("NoCollision"));
}
