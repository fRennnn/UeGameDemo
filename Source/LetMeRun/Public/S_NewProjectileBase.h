// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "S_NewProjectileBase.generated.h"

class USActionEffect;
/**
 * 
 */
UCLASS()
class LETMERUN_API AS_NewProjectileBase : public ASProjectileBase
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DamageAmount;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<USActionEffect> BurningActionClass;
	
	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void Test();
	virtual void BeginPlay() override;

	
public:
	AS_NewProjectileBase();


protected:
	FTimerHandle TimerHandle;
};
