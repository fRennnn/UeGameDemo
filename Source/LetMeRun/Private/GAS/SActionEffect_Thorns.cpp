// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/SActionEffect_Thorns.h"
#include "SAttributeComponent.h"
#include "GAS/SActionComponent.h"
#include "SGameplayFunctionLibrary.h"


USActionEffect_Thorns::USActionEffect_Thorns()
{
	ReflectFraction = 0.2f;

	// Set this effect to be indefinite.
	Duration = 0.0f;
	Period = 0.0f;
}

void USActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	// Bind to player's Attribute's Component and start listening
	USAttributeComponent* Attributes = USAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (Attributes)
	{
		Attributes->OnHealthChanged.AddDynamic(this, &USActionEffect_Thorns::OnHealthChanged);
	}
}

void USActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	// Stop Listening
	USAttributeComponent* Attributes = USAttributeComponent::GetAttributes(GetOwningComponent()->GetOwner());
	if (Attributes)
	{
		Attributes->OnHealthChanged.RemoveDynamic(this, &USActionEffect_Thorns::OnHealthChanged);
	}
}

void USActionEffect_Thorns::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	AActor* OwningActor = GetOwningComponent()->GetOwner();

	// 只处理受伤
	if (Delta < 0.0f && OwningActor != InstigatorActor)
	{
		// 四舍五入防止奇怪的小数出现
		int32 ReflectedAmount = FMath::RoundToInt(Delta * ReflectFraction);
		if (ReflectedAmount == 0)
		{
			return;
		}

		// 反伤值设置为正数, 使用ApplyDamage造成伤害
		ReflectedAmount = FMath::Abs(ReflectedAmount);

		// 反伤给来源
		USGameplayFunctionLibrary::ApplyDamage(OwningActor, InstigatorActor, ReflectedAmount);
	}
}
