// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractActor/SPowerupActor.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASPowerupActor::ASPowerupActor()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Test");
	RootComponent = SphereComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);

	RespawnTime = 10.0f;
	bIsActive = true;
	
	// Tell the server Spawn this Actor and all the client the spawn a SAME actor,ana SAME Location.
	SetReplicates(true);
}

// Called when the game starts or when spawned
void ASPowerupActor::BeginPlay()
{
	Super::BeginPlay();
}


// Function: Let this actor disappear for a while and respawn.
void ASPowerupActor::HideAndCooldownPowerup()
{
	SetPowerupState(false);
	GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &ASPowerupActor::ShowPowerup, RespawnTime);
}

void ASPowerupActor::ShowPowerup()
{
	SetPowerupState(true);
}

void ASPowerupActor::SetPowerupState(bool bNewIsActive)
{
	bIsActive = bNewIsActive;
	OnRep_IsActive();
}

void ASPowerupActor::Interact_Implementation(APawn* InstigatorPawn)
{
	
}

void ASPowerupActor::OnRep_IsActive()
{
	SetActorEnableCollision(bIsActive);
	// Set Visibility
	RootComponent->SetVisibility(bIsActive, true);
}

// Called every frame
void ASPowerupActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASPowerupActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPowerupActor, bIsActive);
}



