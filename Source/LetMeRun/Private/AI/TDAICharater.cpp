// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/TDAICharater.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "SAttributeComponent.h"
#include "SWorldUserWidget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AISense_Damage.h"

ATDAICharacter::ATDAICharacter()
{
	UAIPerceptionSystem* PerceptionSys = UAIPerceptionSystem::GetCurrent(GetWorld());
    
}

void ATDAICharacter::SetTargetActor(AActor* TargetActor)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor",TargetActor);
	}
}


void ATDAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	UE_LOG(LogTemp, Warning, TEXT("ATDAICharacter::PostInitializeComponents"));
	AttributeComp->OnHealthChanged.AddDynamic(this,&ATDAICharacter::OnHealthChanged);
}

void ATDAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	UE_LOG(LogTemp,Error,TEXT("OnHealthChanged in ATDAICharater"));
	//  灼烧效果导致Instigator Actor & OwningComp 为 Null
	//  已解决，在ActionEffect的ExecutePeriodicEffect_Implementation中把Instigator连到ApplyDamage的DamageCauser中
	if (Delta < 0.0f)
	{
		UAISense_Damage::ReportDamageEvent(GetWorld(),
			this,InstigatorActor,FMath::Abs(Delta),
			InstigatorActor->GetActorLocation(),{0,0,0},"A");
		if (InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
		}
     		
		if (ActiveHealthBar == nullptr)
		{
			ActiveHealthBar = CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (ActiveHealthBar)
			{
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}else
			{
				UE_LOG(LogTemp,Error,TEXT("Hp Widget Death"));
			}
		}
     		
		// If AI die
		if (NewHealth <= 0.0f)
		{
			// stop BT
			AAIController* AIC = Cast<AAIController>(GetController());
			if (AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");
			}
     
			// ragdoll
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");
     
			GetCapsuleComponent()->SetCollisionEnabled((ECollisionEnabled::Type::NoCollision));
			GetCharacterMovement()->DisableMovement();
     			
			// set lifespan
			SetLifeSpan(5.0f);
		}
	}
}
