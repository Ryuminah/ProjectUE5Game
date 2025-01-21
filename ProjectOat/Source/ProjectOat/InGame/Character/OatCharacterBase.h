// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InGame/Interface/OatAnimationAttackInterface.h"
#include "Shared/Interface/OatActorWidgetInterface.h"

#include "OatCharacterBase.generated.h"

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater
};

UCLASS()
class PROJECTOAT_API AOatCharacterBase : public ACharacter, public IOatAnimationAttackInterface, public IOatActorWidgetInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AOatCharacterBase();

	virtual void PostInitializeComponents() override;

protected:
	virtual void SetCharacterControlData(const class UOatCharacterControlData* CharcterControlData);

	UPROPERTY(Category=CharacterControl, EditAnywhere, meta=(AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UOatCharacterControlData*> CharacterControlManager;

/* Combo Action -----------------------------------------------------*/
protected:
	UPROPERTY(Category=Animation, EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UAnimMontage> AttackMontage;

	UPROPERTY(Category=Attack, EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UOatAttackActionData> AttackActionData;

	void ProcessAttack();
	void AttackActionMontageBegin();
	void AttackActionMontageEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);


	// 현재까지 진행된 콤보 수
	int32 CurrentCombo = 0;
	// 원하는 시간에 특정 함수 호출
	FTimerHandle ComboTimerHandle;
	// 입력 커맨드가 들어왔는지
	bool bHasNextComboCommand = false;
	void SetComboCheckTimer();
	void ComboCheck();

/* Hit Check -----------------------------------------------------*/
protected:
	virtual void AttackHitCheck() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

/* Dead ----------------------------------------------------------*/
protected:
	UPROPERTY(Category=Animation, EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UAnimMontage> DeadMontage;

	virtual void SetDead();
	void PlayDeadAnim();

	float DeadEventDelayTime = 3.f;

/* Stat ---------------------------------------------------------*/
	UPROPERTY(Category=Stat, VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UOatCharacterStatComponent> Stat;

/* Widget --------------------------------------------------------*/
	UPROPERTY(Category=Widget, VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UOatWidgetComponent> WidgetHpBar;

	virtual void SetUpActorWidget(class UOatUserWidget* InUserWidget) override;
};
