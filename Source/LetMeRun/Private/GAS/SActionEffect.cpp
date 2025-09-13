// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/SActionEffect.h"

#include "GameFramework/GameStateBase.h"
#include "GAS/SActionComponent.h"

USActionEffect::USActionEffect()
{
	bAutoStart = true;
}

void USActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	
	/*
	 * Trigger this effect every Period seconds for Duration
	 * Example : Duration = 10.f  Periods = 2.f
	 * This mean Trigger this effect every 2 seconds for 10s
	 * If set Duration or Period to 0,These if statements below will never work.
	 */
	if (Duration > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StopAction", Instigator);

		GetWorld()->GetTimerManager().SetTimer(DurationHandle, Delegate, Duration, false);
	}

	if (Period > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodicEffect", Instigator);

		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, Delegate, Period, true);
	}
}

void USActionEffect::StopAction_Implementation(AActor* Instigator)
{
	if (GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(Instigator);
	}
	Super::StopAction_Implementation(Instigator);

	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);
	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);

	USActionComponent* Comp = GetOwningComponent();
	if (Comp)
	{
		Comp->RemoveAction(this);
	}
}

float USActionEffect::GetTimeRemaining() const
{
	AGameStateBase* GS = GetWorld()->GetGameState<AGameStateBase>();
	if (GS)
	{
		float EndTime = TimeStarted + Duration;
		return EndTime - GS->GetServerWorldTimeSeconds();
	}

	return Duration;
}

// override this in Blueprint
void USActionEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{ 
	
}


