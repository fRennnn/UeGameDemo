// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractActor/SPowerup_HealthPotion.h"

#include "SAttributeComponent.h"
#include "Assignment5/SPlayerState.h"

class ASPlayerState;

void ASPowerup_HealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
	// Check User's HP is full or not
	if (ensure(AttributeComp) && !AttributeComp->IsFullHealth())
	{
		if (ASPlayerState* PS = InstigatorPawn->GetPlayerState<ASPlayerState>())
		{
			if (PS->RemoveCredits(CreditCost) && AttributeComp->ApplyHealthChange(this, AttributeComp->GetHealthMax()))
			{
				// Only activate if healed successfully
				HideAndCooldownPowerup();
			}
		}
	}
}

ASPowerup_HealthPotion::ASPowerup_HealthPotion()
{
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;
	CreditCost = 5;
}
