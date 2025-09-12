// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SMonsterData.generated.h"

class USAction;
/**
 * 
 */
UCLASS()
class LETMERUN_API USMonsterData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Spawn Info")
	TSubclassOf<AActor> MonsterClass;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Spawn Info")
	TArray<TSubclassOf<USAction>> Actions;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="UI")
	UTexture2D* Icon;

	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("Monsters", GetFName());
	}
};
