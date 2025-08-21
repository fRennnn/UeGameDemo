// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/SAction.h"
#include "SActionEffect.generated.h"

/**
 * 
 */
UCLASS()
class LETMERUN_API USActionEffect : public USAction
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Effect")
	float Duration;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Effect")
	float Period;

	FTimerHandle PeriodHandle;
	FTimerHandle DurationHandle;

	UFUNCTION(BluePrintNativeEvent,Category="Effect")
	void ExecutePeriodicEffect(AActor* Instigator);

public:
	USActionEffect();

	void StartAction_Implementation(AActor* Instigator) override;

	void StopAction_Implementation(AActor* Instigator) override;
};
