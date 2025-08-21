// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SBTService_LowHp.generated.h"

/**
 * 
 */
UCLASS()
class LETMERUN_API USBTService_LowHp : public UBTService
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere,Category="AI")
	float LowHpFraction = 0.5f;
	
	UPROPERTY(EditAnywhere,Category="AI")
	FBlackboardKeySelector SelectedKey;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
