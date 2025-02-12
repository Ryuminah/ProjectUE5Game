// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OatStageSectionTrigger.generated.h"

DECLARE_DELEGATE(FOnStageSectionStateChangedDelegate);
USTRUCT(BlueprintType)
struct FStageSectionChangedDelegateWrapper
{
	GENERATED_BODY()
	FStageSectionChangedDelegateWrapper() {}
	FStageSectionChangedDelegateWrapper(const FOnStageSectionStateChangedDelegate& InDelegate) : SectionDelegate(InDelegate){}

	FOnStageSectionStateChangedDelegate SectionDelegate;
};

UENUM(BlueprintType)
enum class  EStageSectionState : uint8
{
	READYBATTLE = 0,
	INBATTLE = 1,
	ENDBATTLE = 2,
	NONE = 3,
};


UCLASS()
class PROJECTOAT_API AOatStageSectionTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOatStageSectionTrigger();

protected:
	virtual void BeginPlay() override;

/* Stage ------------------------------------------------------*/
protected:
	UPROPERTY(Category=Stage, VisibleAnywhere, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> StageTrigger;

	// ���� ���������� ���� ����
	UPROPERTY(Category=Stage, EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	int SectionId = 0;

	UFUNCTION()
	void OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
									bool bFromSweep, const FHitResult& SweepHitResult);


/* Spawn ------------------------------------------------------*/
	UPROPERTY(Category = HUD, EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class AOatSpawnPoint> OatSpawnPointClass;

	UPROPERTY()
	TArray<TObjectPtr<class AOatSpawnPoint>> SpawnPointArray;
	void SpawnSectionEnemy(FVector SpawnPos/*���� ���� ���̺� ���ڷ�*/);

	void CreateSpawnPointData();

// Gate�� ���� �����
/* Gate ------------------------------------------------------*/
	UPROPERTY(Category=Stage, VisibleAnywhere, meta=(AllowPrivateAccess = "true"))
	TMap<FName, TObjectPtr<class UStaticMeshComponent>> GateMap;

	UPROPERTY(Category=Stage, VisibleAnywhere, meta=(AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UBoxComponent>> GateTriggerArray;

	UFUNCTION()
	void OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
								   bool bFromSweep, const FHitResult& SweepHitResult);

/* Section State ------------------------------------------------------*/
private:
	
	UPROPERTY(Category=Stage, EditAnywhere, meta=(AllowPrivateAccess = "true"))
	EStageSectionState CurrentState;

	void SetSectionState(EStageSectionState InNewState);

	UPROPERTY()
	TMap<EStageSectionState, FStageSectionChangedDelegateWrapper> SectionStateChangedCallback;
	void SetReadyBattle();
	void SetInBattle();
	void SetEndBattle();


/* Battle State ------------------------------------------------------*/
	// TSubClassOf -> �𸮾����� �����ϴ� ���ø� : ������ Ŭ�����κ��� ��ӹ��� Ŭ���� ��ϸ� ǥ���ϵ���
	UPROPERTY(Category=Battle, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AOatCharacterNPC> OpponentClass;

	UFUNCTION()
	void OnOpponentDestroyed(AActor* DestroyedActor);
};
