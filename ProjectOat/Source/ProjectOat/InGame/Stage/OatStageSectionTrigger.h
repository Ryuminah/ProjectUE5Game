// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shared/Enums.h"
#include "OatStageSectionTrigger.generated.h"


DECLARE_DELEGATE(FOnStageSectionStateChangedDelegate)

USTRUCT(BlueprintType)
struct FStageSectionChangedDelegateWrapper
{
	GENERATED_BODY()
	FStageSectionChangedDelegateWrapper() {}
	FStageSectionChangedDelegateWrapper(const FOnStageSectionStateChangedDelegate& InDelegate) : SectionDelegate(InDelegate){}

	FOnStageSectionStateChangedDelegate SectionDelegate;
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
	virtual void PostInitializeComponents() override;


/* Stage ------------------------------------------------------*/
protected:
	UPROPERTY(Category=Stage, VisibleAnywhere, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> StageTrigger;

	// ���� ���������� ���� ����
	UPROPERTY(Category=Stage, EditAnywhere, meta=(AllowPrivateAccess = "true"))
	int SectionId = 0;
	
	UFUNCTION()
	void OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
									bool bFromSweep, const FHitResult& SweepHitResult);


/* Spawn ------------------------------------------------------*/
	UPROPERTY(Category = Spawn, BlueprintReadOnly)
	TSubclassOf<class AOatSpawnPoint> OatSpawnPointClass;

	UPROPERTY()
	TArray<TSoftObjectPtr<class AOatSpawnPoint>> SpawnPointArray;

	void SpawnSectionEnemy(FVector SpawnPos/*,���� ���� ���̺�*/);
	void CreateSpawnPointData();
	
/* Section State ------------------------------------------------------*/
private:
	UPROPERTY(Category=Stage, EditAnywhere, meta=(AllowPrivateAccess = "true"))
	EStageSectionState CurrentState;
	void SetSectionState(EStageSectionState NewState);

	UPROPERTY()
	TMap<EStageSectionState, FStageSectionChangedDelegateWrapper> SectionStateChangedCallback;

private:
	void SetReadyBattle();
	void SetInBattle();
	void SetEndBattle();


/* Battle State ------------------------------------------------------*/
	// TSubClassOf -> �𸮾����� �����ϴ� ���ø� : ������ Ŭ�����κ��� ��ӹ��� Ŭ���� ��ϸ� ǥ���ϵ���
	// �ӽ÷� �Ͻ� �־����
	UPROPERTY(Category=Battle, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AOatCharacterNPC> OpponentClass;

	UFUNCTION()
	void OnOpponentDestroyed(AActor* DestroyedActor);
};

// // Gate�� ���� �����
// /* Gate ------------------------------------------------------*/
// 	UPROPERTY(Category=Stage, VisibleAnywhere, meta=(AllowPrivateAccess = "true"))
// 	TMap<FName, TObjectPtr<class UStaticMeshComponent>> GateMap;
//
// 	UPROPERTY(Category=Stage, VisibleAnywhere, meta=(AllowPrivateAccess = "true"))
// 	TArray<TObjectPtr<class UBoxComponent>> GateTriggerArray;
//
// 	UFUNCTION()
// 	void OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
// 								   bool bFromSweep, const FHitResult& SweepHitResult);
