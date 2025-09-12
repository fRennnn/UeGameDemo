// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "SGameModeBase.generated.h"


class USMonsterData;
class ASPlayerState;
class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;
class USSaveGame;
class UDataTable;

// DataTable Row for spawn monsters in game mode
USTRUCT()
struct FMonsterInfoRow : public FTableRowBase
{
	GENERATED_BODY()
public:

	FMonsterInfoRow()
	{
		Weight = 1.f;
		SpawnCost = 5.f;
		KillReward = 20.f;
	}

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FPrimaryAssetId MonsterID;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float Weight;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float SpawnCost;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float KillReward;
};
/**
 * 
 */
UCLASS()
class LETMERUN_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	ASGameModeBase();
protected:
	
	FTimerHandle TimerHandle_SpawnBots;
	FTimerHandle TimerHandle_SpawnPowerUps;

	UPROPERTY()
	USSaveGame* CurrentSaveGame;

	UPROPERTY(EditDefaultsOnly,Category="AI")
	UDataTable* MonsterTable;

	FString SlotName;

	/*UPROPERTY(EditDefaultsOnly,Category="AI")
	TSubclassOf<AActor> MinionClass;*/
	
	UPROPERTY(EditDefaultsOnly,Category="AI")
	float SpawnTimerInterval;

	UPROPERTY(EditDefaultsOnly,Category="AI")
	UEnvQuery* SpawnBotQuery;
	
	UPROPERTY(EditDefaultsOnly,Category="PowerUps")
	UEnvQuery* PowerupSpawnQuery;

	// Array of PowerUp Classes.
	UPROPERTY(EditDefaultsOnly,Category="PowerUps")
	TArray<TSubclassOf<AActor>> PowerupClasses;

	/* 拾取物之间的最小间隔距离 */
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	float RequiredPowerupDistance;

	/* 游戏开始时生成的拾取物的数量 */
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	int32 DesiredPowerupCount;

	UPROPERTY(EditDefaultsOnly,Category="AI")
	UCurveFloat* DifficutlyCurve;

	UFUNCTION()
	void SpawnPowersUpElapsed();
	
	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);
	
	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void OnPowerupSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	void OnMonsterLoaded(FPrimaryAssetId LoadedID,FVector SpawnLocation);
	// Credit Value
	int Credit = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	int32 CreditsPerKill;

	
public:
	virtual void StartPlay() override;

	void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer);
	virtual void OnActorKilled(AActor* VictimActor,AActor* Killer);
	UFUNCTION(Exec)
	void KillAllBot();

	UFUNCTION(BluePrintCallable)
	void WriteSaveGame();
 
	void LoadSaveGame();
};
