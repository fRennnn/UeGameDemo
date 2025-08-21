// Fill out your copyright notice in the Description page of Project Settings.


#include "S_NewProjectileBase.h"

#include "SAttributeComponent.h"
#include "Components/SphereComponent.h"
#include "GAS/SActionComponent.h"
#include "GAS/SActionEffect.h"
#include "Kismet/GameplayStatics.h"

void AS_NewProjectileBase::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	USActionComponent* ActionComp = Cast<USActionComponent>(OtherActor->GetComponentByClass(USActionComponent::StaticClass()));
	if (OtherActor && OtherActor != GetInstigator())
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());
		if (USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass())))
		{
			AttributeComp->ApplyHealthChange(GetInstigator(),-DamageAmount);
			UGameplayStatics::PlayWorldCameraShake(this,ImpactShake,GetActorLocation(),ImpactShakeInnerRadius, ImpactShakeOuterRadius);
		
			Explode();

			// Add Burnning Effect to Actor we hit.
			if (ActionComp && HasAuthority())
			{
				ActionComp->AddAction(GetInstigator(),BurningActionClass);
			}
		}
	}
}

void AS_NewProjectileBase::Test()
{
	Destroy();
	//UE_LOG(LogTemp, Error, TEXT("Destroy this actor!!!"));
}

void AS_NewProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	APawn* player = GetInstigator();
	SphereComp->IgnoreActorWhenMoving(player,true);

	// Delete this projectile after 5 seconds.
	GetWorldTimerManager().SetTimer(TimerHandle,this,&AS_NewProjectileBase::Test,5.f);
}

AS_NewProjectileBase::AS_NewProjectileBase()
{
	SphereComp->SetSphereRadius(20.0f);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AS_NewProjectileBase::OnActorOverlap);

	DamageAmount = 20.0f;
}
