// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExplosiveBarrel.generated.h"

class URadialForceComponent;

UCLASS()
class LETMERUN_API AExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExplosiveBarrel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	URadialForceComponent* RadialForce;


	
	
	/**
	* Called when the barrel's mesh component is hit by another object.
	* Handles collision detection and triggers explosion if conditions are met.
	*
	* @param HitComponent		The primitive component that was hit (our barrel mesh)
	* @param OtherActor		    The actor that hit us (projectile, player, etc.)
	* @param OtherComp			The primitive component of the other actor that hit us
	* @param NormalImpulse		The force applied to our component at the hit location
	* @param Hit				Detailed hit result information including location, normal, etc.
	*/
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	           UPrimitiveComponent* OtherComp, FVector NormalImpulse,
	           const FHitResult& Hit);
};
