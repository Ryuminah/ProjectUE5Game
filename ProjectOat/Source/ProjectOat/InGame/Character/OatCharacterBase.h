// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InGame/Interface/OatAnimationAttackInterface.h"

#include "OatCharacterBase.generated.h"

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater
};

UCLASS()
class PROJECTOAT_API AOatCharacterBase : public ACharacter, public IOatAnimationAttackInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AOatCharacterBase();

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


	// ������� ����� �޺� ��
	int32 CurrentCombo = 0;
	// ���ϴ� �ð��� Ư�� �Լ� ȣ��
	FTimerHandle ComboTimerHandle;
	// �Է� Ŀ�ǵ尡 ���Դ���
	bool bHasNextComboCommand = false;
	void SetComboCheckTimer();
	void ComboCheck();

/* Hit Check -----------------------------------------------------*/
protected:
	virtual void AttackHitCheck();

};
