// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/SBTService_LowHp.h"

#include "AIController.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_LowHp::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ensure(AIPawn))
	{
		USAttributeComponent* AttributeComp  = USAttributeComponent::GetAttributes(AIPawn);
		if (AttributeComp)
		{
			bool bLowHealth = (AttributeComp->GetHealth() / AttributeComp->GetHealthMax()) <= LowHpFraction;

			UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
			BBComp->SetValueAsBool(SelectedKey.SelectedKeyName,bLowHealth);
		}
	}
}
