// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USAttributeComponent;
class USInteractionComponent;
struct FInputActionValue;
class USpringArmComponent;
class UCameraComponent;
class UInputComponent;
class UInputAction;
class UInputMappingContext;
class USActionComponent;
UCLASS()
class LETMERUN_API ASCharacter : public ACharacter
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere,Category = "Attack")
	TSubclassOf<class AActor> ProjectileClass;

	UPROPERTY(EditAnywhere,Category = "Attack")
	TSubclassOf<class AActor> BlackHoleProjectileClass;
	
	UPROPERTY(EditAnywhere,Category = "Attack")
	UAnimMontage* AttackAnim;

public:
	ASCharacter();

	UFUNCTION(Exec)
	void HealSelf(float AMount);

	UFUNCTION(Exec)
	void HurtSelf(float AMount);
protected:
	virtual void BeginPlay() override;

	virtual FVector GetPawnViewLocation() const override;
public:	

	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	USActionComponent* ActionComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	USAttributeComponent* AttributeComp;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="PlayerInput",meta=(AllowPrivateAccess=true))
	UInputMappingContext* PlayerInputMapping;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="PlayerInput",meta=(AllowPrivateAccess=true))
	UInputAction* PlayerMove;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="PlayerInput",meta=(AllowPrivateAccess=true))
	UInputAction* PlayerJump;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="PlayerInput",meta=(AllowPrivateAccess=true))
	UInputAction* PlayerLook;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="PlayerInput",meta=(AllowPrivateAccess=true))
	UInputAction* PlayerShoot;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="PlayerInput",meta=(AllowPrivateAccess=true))
	UInputAction* PlayerInteraction;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="PlayerInput",meta=(AllowPrivateAccess=true))
	UInputAction* PlayerSkill;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="PlayerInput",meta=(AllowPrivateAccess=true))
	UInputAction* PlayerSprint;

	virtual void PostInitializeComponents() override;


 	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void SpawnProjectile(TSubclassOf<class AActor> ClassToSpawn);
	void PrimaryAttack();
	void PrimaryUltimate();
	void PrimaryInteract();
	void SprintStart();
	void SprintEnd();
	void DrawDebugLine();
	bool GetScreenCenterToWorld(FVector& OutLocation, FVector& OutRotation) const;
	FVector LineTrace(FVector& Start,FVector& Direction);
};

