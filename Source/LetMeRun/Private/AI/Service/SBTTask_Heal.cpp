// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/SBTTask_Heal.h"

#include "AIController.h"
#include "SAttributeComponent.h"

class USAttributeComponent;

EBTNodeResult::Type USBTTask_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* MyPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (MyPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(MyPawn);
	if (ensure(AttributeComp))
	{
		AttributeComp->ApplyHealthChange(MyPawn, AttributeComp->GetHealthMax());
		UE_LOG(LogTemp,Display,TEXT("Healing"));
	}
	return EBTNodeResult::Succeeded;
}
