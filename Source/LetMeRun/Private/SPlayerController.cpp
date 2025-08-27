// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerController.h"

#include "EnhancedInputComponent.h"
#include "InputState.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerState.h"

void ASPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	OnPawnChanged.Broadcast(InPawn);
}

void ASPlayerController::BeginPlayingState()
{
	BlueprintBeginPlayingState();
}


void ASPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	OnPlayerStateReceived.Broadcast(PlayerState);
}

void ASPlayerController::TogglePauseMenu()
{
	if (PauseMenuInstance && PauseMenuInstance->IsInViewport())
	{
		PauseMenuInstance->RemoveFromParent();
		PauseMenuInstance = nullptr;

		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
		{
			UE_LOG(LogTemp, Warning, TEXT("BINDING ACTION "));
			EnhancedInputComponent->BindAction(PlayerPause,ETriggerEvent::Triggered,this,&ASPlayerController::TogglePauseMenu);
		}
		return;
	}

	PauseMenuInstance = CreateWidget<UUserWidget>(this, PauseMenuClass);
	if (PauseMenuInstance)
	{
		PauseMenuInstance->AddToViewport(100);
		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
		{
			UE_LOG(LogTemp, Warning, TEXT("CLEAR ACTION BINDING"));
			EnhancedInputComponent->ClearActionBindings();
		}
	}
}

void ASPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(PlayerPause,ETriggerEvent::Triggered,this,&ASPlayerController::TogglePauseMenu);
	}
}


