// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BaseAICharacter.h"

#include "SAttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "GAS/SActionComponent.h"

// Sets default values
ABaseAICharacter::ABaseAICharacter()
{
	AttributeComp  = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
	ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");
}


