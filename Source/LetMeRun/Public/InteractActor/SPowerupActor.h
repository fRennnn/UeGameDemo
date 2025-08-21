// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SPowerupActor.generated.h"

class USphereComponent;
class UStaticMeshComponent;
UCLASS()
class LETMERUN_API ASPowerupActor : public AActor ,public ISGameplayInterface
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, Category = "Powerup")
	float RespawnTime;

	FTimerHandle TimerHandle_RespawnTimer;

	UFUNCTION()
	void ShowPowerup();

	void HideAndCooldownPowerup();

	void SetPowerupState(bool bNewIsActive);

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(ReplicatedUsing="OnRep_IsActive")
	bool bIsActive;

	UFUNCTION()
	void OnRep_IsActive();

public:	
	// Sets default values for this actor's properties
	ASPowerupActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
