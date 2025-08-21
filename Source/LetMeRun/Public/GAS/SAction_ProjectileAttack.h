// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/SAction.h"
#include "SAction_ProjectileAttack.generated.h"

class UAnimMontage;

/**
 * 
 */
UCLASS()
class LETMERUN_API USAction_ProjectileAttack : public USAction
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere,Category = "Attack")
	TSubclassOf<class AActor> ProjectileClass;
	
	UPROPERTY(EditAnywhere,Category = "Attack")
	UAnimMontage* AttackAnim;

	/* Particle System played during attack animation */
	UPROPERTY(EditAnywhere, Category = "Attack")
	UParticleSystem* CastingEffect;
	
	UPROPERTY(EditAnywhere,Category = "Attack")
	float AttackAnimDelay;
	
	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);

	FName HandSocketName;
public:
	virtual void StartAction_Implementation(AActor* Instigator) override;

	USAction_ProjectileAttack();
};
