// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosiveBarrel.h"

#include "SAttributeComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

class USAttributeComponent;
// Sets default values
AExplosiveBarrel::AExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetSimulatePhysics(true);
	StaticMesh->SetCollisionProfileName("PhysicsActor");
	StaticMesh->OnComponentHit.AddDynamic(this,&AExplosiveBarrel::OnHit);
	
	RadialForce = CreateDefaultSubobject<URadialForceComponent>("RadialForce");
	RadialForce->SetupAttachment(StaticMesh);
	RadialForce->Radius = 700.f;
	RadialForce->ImpulseStrength = 1000.f;
	RadialForce->bImpulseVelChange = true;

	SetReplicates(true);
	
}

// Called when the game starts or when spawned
void AExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExplosiveBarrel::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	// Bug : 角色碰撞之后有概率触发两次事件。
	/*UE_LOG(LogTemp, Log, TEXT("this: %s"), *GetName());
	UE_LOG(LogTemp, Log, TEXT("HitComponent: %s"), HitComponent ? *HitComponent->GetName() : TEXT("null"));
	UE_LOG(LogTemp, Log, TEXT("OtherActor: %s"), OtherActor ? *OtherActor->GetName() : TEXT("null"));
	UE_LOG(LogTemp, Log, TEXT("OtherComp: %s"), OtherComp ? *OtherComp->GetName() : TEXT("null"));*/
	if (OtherActor && OtherActor != this)
	{
		RadialForce->FireImpulse();
		if (USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass())))
		{
			AttributeComp->ApplyHealthChange(this,-20);
			//Destroy();
		}
	}
}

