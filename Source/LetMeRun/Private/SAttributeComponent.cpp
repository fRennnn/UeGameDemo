// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"
#include "SGameModeBase.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	HealthMax = 100;
	Health = HealthMax;

	RageMax = 100;
	Rage = 0;

	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void USAttributeComponent::BeginPlay()
{

}




bool USAttributeComponent::IsAlive() const
{
	return Health > 0.f;
}


// Called every frame
void USAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor,float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}
	// For debug
	if (Delta < 0.f)
	{
		float DamagedMultiplier = CVarDamageMultiplier.GetValueOnGameThread();
		Delta *= DamagedMultiplier;
	}
	
	float OldHealth = Health;
	// set Health -> [0 , HealthMax]
	float NewHealth = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	float ActualDelta = NewHealth - OldHealth;

	// Is Server?
	if (GetOwner()->HasAuthority())
	{
		Health = NewHealth;
		
		if (ActualDelta != 0.0f)
		{
			MulticastHealthChanged(InstigatorActor, Health, ActualDelta);
		}

		// If the Actor Died
		if (ActualDelta <0.f && Health == 0.f)
		{
			ASGameModeBase* GM = GetWorld()->GetAuthGameMode<ASGameModeBase>();
			if (GM)
			{
				GM->OnActorKilled(GetOwner(), InstigatorActor);
			}
		}
	}
	
	return ActualDelta != 0;
}

bool USAttributeComponent::ApplyRage(AActor* InstigatorActor, float Delta)
{
	/*float OldHealth = Health;
	// set Health -> [0 , HealthMax]
	float NewHealth = FMath::Clamp(Health + Delta, 0.0f, HealthMax);
	float ActualDelta = NewHealth - OldHealth;*/

	float OldRage = Rage;
	// set Rage -> [0 , RageMax]
	float NewRage = FMath::Clamp(Rage + Delta, 0.0f, RageMax);
	float ActualDelta = NewRage - OldRage;
	if (GetOwner()->HasAuthority())
	{
		Rage =	NewRage;
		FString FailedMsg = FString::Printf(TEXT("ActualDelta : %f"),ActualDelta);
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FailedMsg);
		if (ActualDelta != 0.0f)
		{
			MulticastRageChanged(InstigatorActor, Rage, ActualDelta);
		}
	}
	
	return ActualDelta != 0;
}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<USAttributeComponent>(FromActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	}
	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	USAttributeComponent* AttributeComp = GetAttributes(Actor);
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}
	return false;
}

void USAttributeComponent::MulticastRageChanged_Implementation(AActor* InstigatorActor, float NewRage, float Delta)
{
	OnRageChanged.Broadcast(InstigatorActor, this, NewRage, Delta);
}

bool USAttributeComponent::IsFullHealth() const
{
	return Health == HealthMax;
}

float USAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}

float USAttributeComponent::GetHealth() const
{
	return Health;
}

float USAttributeComponent::Kill(AActor* Instigator)
{
	return ApplyHealthChange(Instigator,-GetHealthMax());
}

float USAttributeComponent::GetRage() const
{
	return Rage;
}

void USAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
}

void USAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAttributeComponent, Health);
	DOREPLIFETIME(USAttributeComponent, HealthMax);

	DOREPLIFETIME(USAttributeComponent, Rage);
	DOREPLIFETIME(USAttributeComponent, RageMax);
	//DOREPLIFETIME_CONDITION(USAttributeComponent, HealthMax, COND_InitialOnly);
}





