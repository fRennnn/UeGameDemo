// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class UPawnSensingComponent;
class USAttributeComponent;
class UUserWidget;
class USWorldUserWidget;
class USActionComponent;
UCLASS()
class LETMERUN_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()
protected:

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	USAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	USActionComponent* ActionComp;
	
	UPROPERTY(VisibleAnywhere,Category="Components")
	UPawnSensingComponent* PawnSensingComp;
	
	UPROPERTY(EditAnywhere,Category = "Attack")
	UAnimMontage* AttackAnim;

	USWorldUserWidget* ActiveHealthBar;

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPawnSeen();

	/**
	 *  When Enemy Found Player,This work.
	 */
	USWorldUserWidget* SpottedBar;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> SpottedWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	FName TargetActorKey;

	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetTargetActor(AActor* NewTarget);

	UFUNCTION(BlueprintCallable, Category = "AI")
	AActor* GetTargetActor() const;
	
	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

public:
	// Sets default values for this character's properties
	ASAICharacter();
	
	UFUNCTION()
	void PrimaryAttack();
};
