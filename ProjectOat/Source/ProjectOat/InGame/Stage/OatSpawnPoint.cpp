// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Stage/OatSpawnPoint.h"
#include "Components/BoxComponent.h"


void AOatSpawnPoint::BeginDestroy()
{
	Super::BeginDestroy();
	//UE_LOG(LogTemp, Warning, TEXT("BeginDestroy()"));
}

void AOatSpawnPoint::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// Sets default values
AOatSpawnPoint::AOatSpawnPoint()
{
	SpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnBox"));
	SetRootComponent(SpawnBox);

	SpawnBox->SetBoxExtent(FVector(1.f, 1.f, 1.f));
	SpawnBox->SetCollisionProfileName(TEXT("NoCollision"));
}
