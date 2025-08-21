// Fill out your copyright notice in the Description page of Project Settings.


#include "LetMeRun/Public/InteractActor/S_Credit.h"

#include "Assignment5/SPlayerState.h"
#include "LetMeRun/LetMeRun.h"

class ASPlayerState;
// Sets default values
AS_Credit::AS_Credit()
{
 	CreditCost = 1000;
	RespawnTime = 5.f;
}

// Called when the game starts or when spawned
void AS_Credit::BeginPlay()
{
	Super::BeginPlay();
}

void AS_Credit::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}
	
	/*FString ActionMsg = FString::Printf(TEXT("Running AS_Credit::Interact_Implementation() "));
	LogOnScreen(this,ActionMsg,FColor::Purple);*/
	
	if (ASPlayerState* PS = InstigatorPawn->GetPlayerState<ASPlayerState>())
	{
		FString ActionMsg = FString::Printf(TEXT("InstigatorPawn : %s"),*InstigatorPawn->GetName());
		LogOnScreen(this,ActionMsg,FColor::Purple);
		
		PS->AddCredits(CreditCost);
		HideAndCooldownPowerup();
	}
}

// Called every frame
void AS_Credit::Tick(float DeltaTime)
{

}



