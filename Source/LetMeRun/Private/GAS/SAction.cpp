// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/SAction.h"

#include "GAS/SActionComponent.h"
#include "LetMeRun/LetMeRun.h"
#include "Net/UnrealNetwork.h"

void USAction::StartAction_Implementation(AActor* Instigator)
{
	LogOnScreen(this,FString::Printf(TEXT("Started: %s"),*ActionName.ToString()),FColor::Green);
	
	USActionComponent* Comp = GetOwningComponent();	
	Comp->ActiveGamePlayTags.AppendTags(GrantsTags); // 3:24 这里报错，教程中的Action是临时为了同步做的。和现在的技能不太一样。

	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;
}

void USAction::StopAction_Implementation(AActor* Instigator)
{

	LogOnScreen(this,FString::Printf(TEXT("Stopped: %s"),*ActionName.ToString()),FColor::White);
	//ensureAlways(bIsRunning);
	
	USActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGamePlayTags.RemoveTags(GrantsTags);

	RepData.bIsRunning = false;
	UE_LOG(LogTemp,Error,TEXT("Set RUnning is False"));
}

bool USAction::CanStart_Implementation(AActor* Instigator)
{
	// Refuse to add this action if  action  still running.
	if (IsRunning())
	{
		UE_LOG(LogTemp, Error, TEXT("In IsRunning()"));
		return false;
	}

	USActionComponent* Comp = GetOwningComponent();

	// Check if this Actor have have BlockedTags.If it is true,we stop adding tag to it.
	if (Comp->ActiveGamePlayTags.HasAny(BlockedTags))
	{
		UE_LOG(LogTemp, Error, TEXT("In CheckBlockTags"));
		return false;
	}

	return true;
}

bool USAction::IsRunning() const
{
	return RepData.bIsRunning;
}

UWorld* USAction::GetWorld() const
{
	AActor* Actor = Cast<AActor>(GetOuter());
	if (Actor)
	{
		return Actor->GetWorld();
	}

	return nullptr;
}

USActionComponent* USAction::GetOwningComponent() const
{
	//AActor* Actor = Cast<AActor>(GetOuter());
	//return Actor->GetComponentByClass(USActionComponent::StaticClass());

	return ActionComp;
}

void USAction::Initialize(USActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
}

void USAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAction, RepData);
	DOREPLIFETIME(USAction, ActionComp);
}

void USAction::OnRep_RepData()
{
	if (RepData.bIsRunning)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}
