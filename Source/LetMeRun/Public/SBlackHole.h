// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SBlackHole.generated.h"

class USphereComponent;

class UParticleSystemComponent;

class UProjectileMovementComponent;

class URadialForceComponent;
UCLASS()
class LETMERUN_API ASBlackHole : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASBlackHole();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UParticleSystemComponent* EffectComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UProjectileMovementComponent* MovementComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	URadialForceComponent* RadialForce;
};
