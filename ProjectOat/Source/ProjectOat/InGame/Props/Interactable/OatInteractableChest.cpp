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
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Effect"));

	RootComponent = Trigger;
	Mesh->SetupAttachment(Trigger);
	Effect->SetupAttachment(Trigger);

	Trigger->SetCollisionProfileName(CPROFILE_OATTRIGGER);
	Trigger->SetBoxExtent(FVector(74.f, 32.f, 138.f));
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AOatInteractableChest::OnOverlapBegin);


	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/ProjectOat/Arts/Backgrounds/Structures/GasPump/SM_GasPump.SM_GasPump'"));
	if (BoxMeshRef.Object)
	{
		Mesh->SetStaticMesh(BoxMeshRef.Object);
	}

	Mesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> EffectRef(TEXT("/Script/Engine.ParticleSystem'/Game/ProjectOat/Arts/Shared/VFX/Fx_Item_Drop/PS_ItemDrop.PS_ItemDrop'"));
	if (EffectRef.Object)
	{
		Effect->SetTemplate(EffectRef.Object);
		Effect->bAutoActivate = false;
	}
}

void AOatInteractableChest::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	Effect->Activate();
	Mesh->SetHiddenInGame(true);
	SetActorEnableCollision(false);

	Effect->OnSystemFinished.AddDynamic(this, &AOatInteractableChest::OnEffectFinished);
}

void AOatInteractableChest::OnEffectFinished(UParticleSystemComponent* ParticleSystem)
{
	Destroy();
}


