// Fill out your copyright notice in the Description page of Project Settings.


#include "SBlackHole.h"


#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ASBlackHole::ASBlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile");
	RootComponent = SphereComp;
  
	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 1000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
	

	// Force
	RadialForce = CreateDefaultSubobject<URadialForceComponent>("RadialForce");
	RadialForce->SetupAttachment(SphereComp);
	RadialForce->Radius = 700.f;
	RadialForce->ImpulseStrength = -1000.f;
	RadialForce->bImpulseVelChange = true;

	// Make Sure to SetReplicates to TRUE
	SetReplicates(true);
}

// Called when the game starts or when spawned
void ASBlackHole::BeginPlay()
{
	Super::BeginPlay();

	SphereComp->IgnoreActorWhenMoving(GetInstigator(),true);
}

// Called every frame
void ASBlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

