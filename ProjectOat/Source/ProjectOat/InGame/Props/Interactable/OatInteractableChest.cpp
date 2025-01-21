// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Props/Interactable/OatInteractableChest.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "InGame/Physics/OatCollision.h"


// Sets default values
AOatInteractableChest::AOatInteractableChest()
{
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EffectS"));

	RootComponent = Trigger;
	Mesh->SetupAttachment(Trigger);
	Effect->SetupAttachment(Trigger);

	Trigger->SetCollisionProfileName(CPROFILE_OATTRIGGER);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/ProjectOat/Arts/Backgrounds/Structures/GasPump/SM_GasPump.SM_GasPump'"));
	if (BoxMeshRef.Object)
	{
		Mesh->SetStaticMesh(BoxMeshRef.Object);
	}

	Mesh->SetRelativeLocation(FVector(0.f, -3.5f, -30.f));
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
}


