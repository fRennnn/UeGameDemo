// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BaseAICharacter.h"

#include "SAttributeComponent.h"
#include "GAS/SActionComponent.h"

// Sets default values
ABaseAICharacter::ABaseAICharacter()
{
	AttributeComp  = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
	ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");
}

void ABaseAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	// Do nothing
}


