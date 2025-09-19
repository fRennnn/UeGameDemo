// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/BaseAICharacter.h"
#include "TDAICharater.generated.h"

/**
 * 
 */
class USWorldUserWidget;
class UAISense_Damage;
UCLASS()
class LETMERUN_API ATDAICharacter : public ABaseAICharacter
{
	GENERATED_BODY()
public:
	ATDAICharacter();
protected:
	
	UFUNCTION()
	virtual void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta) override;

	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetTargetActor(AActor* NewTarget);

	// ReSharper disable once CppUE4ProbableMemoryIssuesWithUObject
	USWorldUserWidget* ActiveHealthBar;

	
	virtual void PostInitializeComponents() override;
};
