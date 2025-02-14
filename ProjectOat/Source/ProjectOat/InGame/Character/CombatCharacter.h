// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InGame/Interface/OatAnimationAttackInterface.h"
#include "Shared/Enums.h"
#include "CombatCharacter.generated.h"

USTRUCT()
struct FAttackResult
{
	GENERATED_BODY()

	//FCCEffectInfo CCEffectInfo;
	//FStatusEffectInfo StatusEffectInfo;
};
//
//USTRUCT()
//struct FCCEffectInfo
//{
//	GENERATED_BODY()
//
//	ECCEffectType CCType;
//
//	// �̰� �׳� DA����ü�� �޴°� �������� ������;;
//	// �̵� �ӵ��� ������ ��´�
//	float ModifierMoveSpeedRate;
//
//	// ���� �ð�
//	float Duration;
//
//	// ��ų ��� ��������
//	bool bCanUseAnySkill;
//
//	// ��ų ��� ��������
//	bool bCanAttack;
//};
//
//USTRUCT()
//struct FStatusEffectInfo
//{
//	GENERATED_BODY()
//
//	float ModifierMoveSpeedRate;
//};

UCLASS()
class PROJECTOAT_API ACombatCharacter : public ACharacter, public IOatAnimationAttackInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACombatCharacter();

protected:

/* Anim -----------------------------------------------------*/
protected:
	UPROPERTY(Category=Animation, EditAnywhere, BlueprintReadWrite)
	TMap<FName,TObjectPtr<class UAnimMontage>> AnimMontageMap;


/* Attack -----------------------------------------------------*/
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
};
