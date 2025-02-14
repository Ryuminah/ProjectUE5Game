// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Character/CombatCharacter.h"

// Sets default values
ACombatCharacter::ACombatCharacter()
{
 
}

void ACombatCharacter::AttackHitCheck()
{}

float ACombatCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount,DamageEvent,EventInstigator,DamageCauser);

	return 0.0f;
}

void ACombatCharacter::SetDead()
{}

void ACombatCharacter::PlayDeadAnim()
{}

