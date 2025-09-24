// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "SWorldUserWidget.h"
#include "SGameplayInterface.h"
#include "Blueprint/UserWidget.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("su.InteractionDebugDraw"), false, TEXT("Enable Debug Lines for Interact Component."), ECVF_Cheat);

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	TraceRadius = 30.0f;
	TraceDistance = 500.0f;
	CollisionChannel = ECC_WorldDynamic;
	
	// ...
}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* MyPawn = Cast<APawn>(GetOwner());
	if (MyPawn->IsLocallyControlled())
	{
		FindBestInteratable();
	}
	// ...
}

void USInteractionComponent::FindBestInteratable()
{
	bool bDebugDraw = CVarDebugDrawInteraction.GetValueOnGameThread();

	
	// Initialize
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	
	AActor* MyOwner = GetOwner();
	FVector EyeLocation = GetOwner()->GetActorLocation();
	FRotator Rotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation,Rotation);
	FVector End = EyeLocation + (Rotation.Vector() * TraceDistance);

	// clear rf before trying to fill
	FocusActor = nullptr;
	FHitResult Hit;
	// LineTrace
	bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit,EyeLocation,End,ObjectQueryParams);
	AActor* HitActor = Hit.GetActor();
	if (HitActor)
	{
		if (HitActor->Implements<USGameplayInterface>())
		{
			FocusActor = HitActor;
		}else
		{
			// UE_LOG(LogTemp,Error,TEXT("Oops,No Interactable"));
		}
	}

	// If found this Actor, make its widget visible
	if (FocusActor)
	{
		// Lazy Load???
		if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance = CreateWidget<USWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}

		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachedActor = FocusActor;

			if (!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}
		}
	}else// If not,clear that widget.
	{
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
		
	}
	
	FColor Color = HitActor?FColor::Green : FColor::Red;
	if (bDebugDraw)
	{
		DrawDebugLine(GetWorld(), EyeLocation, End, Color, false, 2.0f, 0, 2.0f);
	}
}

void USInteractionComponent::PrimaryInteract()
{
	ServerInteract(FocusActor);
}

void USInteractionComponent::ServerInteract_Implementation(AActor* InFocus)
{
	if (InFocus == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No Focus Actor to interact.");
		return;
	}

	APawn* MyPawn = Cast<APawn>(GetOwner());

	ISGameplayInterface::Execute_Interact(InFocus, MyPawn);
}

