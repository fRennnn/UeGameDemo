// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "SAction.generated.h"

class USActionComponent;
class UWorld;

USTRUCT()
struct FActionRepData
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	bool bIsRunning;
	
	UPROPERTY()
	AActor* Instigator;
};
/**
 * 
 */
UCLASS(Blueprintable)
class LETMERUN_API USAction : public UObject
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);
 
	/* Action nickname to start/stop without a reference to the object */
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator);
	
	virtual UWorld* GetWorld() const override;

	void Initialize(USActionComponent* NewActionComp);
	
	/* Set this Action AutoStart when we add this to the ActionComponent */
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart;
	
protected:

	UPROPERTY(Replicated)
	USActionComponent* ActionComp;
	
	/* Tags added to owning actor when activated, removed when action stops */
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	/* Action can only start if OwningActor has none of these Tags applied */
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	UFUNCTION(BlueprintCallable, Category = "Action")
	USActionComponent* GetOwningComponent() const;
	
	bool IsSupportedForNetworking() const override
	{
		return true;
	}
	
	UPROPERTY(ReplicatedUsing="OnRep_RepData")
	FActionRepData RepData;

	UFUNCTION()
	void OnRep_RepData();
};
