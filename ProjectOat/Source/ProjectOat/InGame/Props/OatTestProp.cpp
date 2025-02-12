// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Props/OatTestProp.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AOatTestProp::AOatTestProp()
{
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Cactus = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cactus"));

	RootComponent = Body;
	Cactus->SetupAttachment(Body);
	Cactus->SetRelativeLocation(FVector(0.f, 0.f, 72.f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BodyMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/ROXY/Geometry/Meshes/1M_Cube.1M_Cube'"));
	if (BodyMeshRef.Object)
	{
		Body->SetStaticMesh(BodyMeshRef.Object);
	}
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CactusMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/ROXY/Resources/Background/Natural/SM_MERGED_cactus_1.SM_MERGED_cactus_1'"));
	if (CactusMeshRef.Object)
	{
		Cactus->SetStaticMesh(CactusMeshRef.Object);
	}

}

// Called when the game starts or when spawned
void AOatTestProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOatTestProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

