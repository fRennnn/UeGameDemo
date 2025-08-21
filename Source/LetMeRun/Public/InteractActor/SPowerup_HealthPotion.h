// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerupActor.h"
#include "SPowerup_HealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class LETMERUN_API ASPowerup_HealthPotion : public ASPowerupActor
{
	GENERATED_BODY()
	
public:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	
	ASPowerup_HealthPotion();
	
protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	int32 CreditCost;
};
