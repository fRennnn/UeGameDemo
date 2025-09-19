// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAICharacter.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class UPawnSensingComponent;
class USAttributeComponent;
class UUserWidget;
class USWorldUserWidget;
class USActionComponent;
UCLASS()
class LETMERUN_API ASAICharacter : public ABaseAICharacter
{
	GENERATED_BODY()
protected:
	
	UPROPERTY(VisibleAnywhere,Category="Components")
	UPawnSensingComponent* PawnSensingComp;
	
	UPROPERTY(EditAnywhere,Category = "Attack")
	UAnimMontage* AttackAnim;

	// ReSharper disable once CppUE4ProbableMemoryIssuesWithUObject
	USWorldUserWidget* ActiveHealthBar;

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPawnSeen();

	/**
	 *  When Enemy Found Player,This work.
	 */
	// ReSharper disable once CppUE4ProbableMemoryIssuesWithUObject
	USWorldUserWidget* SpottedBar;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> SpottedWidgetClass;

	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetTargetActor(AActor* NewTarget);

	UFUNCTION(BlueprintCallable, Category = "AI")
	AActor* GetTargetActor() const;
	
	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION()
	virtual void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta) override;

public:
	// Sets default values for this character's properties
	ASAICharacter();
	
	UFUNCTION()
	void PrimaryAttack();
};
