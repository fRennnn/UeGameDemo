// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SPlayerController.generated.h"

class UInputAction;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnChanged, APawn*, NewPawn);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChanged, APlayerState*, NewPlayerState);
/**
 * 
 */
UCLASS()
class LETMERUN_API ASPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	// Listen for incoming player state (for clients this may be nullptr when initially joining a game, 
	// afterwards player state will not change again as PlayerControllers maintain the same player state throughout the level)
	UPROPERTY(BlueprintAssignable)
	FOnPlayerStateChanged OnPlayerStateReceived;
	
	UPROPERTY(BlueprintAssignable)
	FOnPawnChanged OnPawnChanged;

	virtual void SetPawn(APawn* InPawn) override;

	/* Called when player controller is ready to begin playing, the good moment to initialize things like UI which might be too early in BeginPlay 
	(esp. in multiplayer clients where not all data such as PlayerState may have been received yet) */
	virtual void BeginPlayingState() override;

	UFUNCTION(BlueprintImplementableEvent)
	void BlueprintBeginPlayingState();

	void OnRep_PlayerState() override;

	//===For Pause Menu===
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="PlayerInput",meta=(AllowPrivateAccess=true))
	UInputAction* PlayerPause;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PauseMenuClass;

	UPROPERTY()
	UUserWidget* PauseMenuInstance;

	UFUNCTION(BlueprintCallable)
	void TogglePauseMenu();

	void SetupInputComponent() override;
	
	//=========================
};
