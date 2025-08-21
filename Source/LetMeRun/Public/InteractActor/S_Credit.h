// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPowerupActor.h"
#include "GameFramework/Actor.h"
#include "S_Credit.generated.h"

UCLASS()
class LETMERUN_API AS_Credit : public ASPowerupActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AS_Credit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	int32 CreditCost;

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
