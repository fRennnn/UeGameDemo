// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include "Constraint.h"
#include "EngineUtils.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "SGameplayInterface.h"
#include "SSaveGame.h"
#include "AI/SAICharacter.h"
#include "Assignment5/SPlayerState.h"
#include "Data/SMonsterData.h"
#include "Engine/AssetManager.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "GameFramework/GameStateBase.h"
#include "GAS/SActionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LetMeRun/LetMeRun.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);


ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval = 2.f;
	Credit = 0;
	CreditsPerKill = 10.f;
	PlayerStateClass = ASPlayerState::StaticClass();

	SlotName = "SaveGame01";
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots,this,&ASGameModeBase::SpawnBotTimerElapsed,SpawnTimerInterval,true);

	if (PowerupClasses.Num() > 0)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_SpawnPowerUps,this,&ASGameModeBase::SpawnPowersUpElapsed,SpawnTimerInterval,false);
	}else
	{
		UE_LOG(LogTemp,Error,TEXT("Can not find the PowerupClasses!!!"));
	}
	
}

void ASGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	LoadSaveGame();

	UE_LOG(LogTemp,Display,TEXT("Loading Game Data"));
}

void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	ASCharacter* Player = Cast<ASCharacter>(VictimActor);
	// If Player Died
	if (Player) 
	{
		FTimerHandle TimerHandle_RespawnDelay;
		Player->SetActorEnableCollision(false);
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this,"RespawnPlayerElapsed",Player->GetController());

		float RespawnDelay = 2.f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay,Delegate,RespawnDelay,false);
	}
	
	// 击杀给予分数
	APawn* KillerPawn = Cast<APawn>(Killer);
	if (KillerPawn)
	{
		ASPlayerState* PS = KillerPawn->GetPlayerState<ASPlayerState>();// failed
		if (PS) 
		{
			UE_LOG(LogTemp,Error,TEXT("AI Dead"));
			PS->AddCredits(CreditsPerKill);
		}
	}
}

void ASGameModeBase::KillAllBot()
{
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;
		
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(Bot->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			AttributeComp->Kill(Bot);
		}
	} 
}

void ASGameModeBase::SpawnPowersUpElapsed()
{
	UEnvQueryInstanceBlueprintWrapper* QueryInstance =  UEnvQueryManager::RunEQSQuery(this,PowerupSpawnQuery,this,EEnvQueryRunMode::RandomBest25Pct,nullptr);
	if (QueryInstance)
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this,&ASGameModeBase::OnPowerupSpawnQueryCompleted);
	}
}

// Spawn AI Bot in the map
void ASGameModeBase::SpawnBotTimerElapsed()
{
	// this is for Debug, true to stop.
	if(CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot spawning disabled via cvar 'CVarSpawnBots'."));
		return;
	}
	
	int32 NrOfAliveBots = 0;
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;
		
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(Bot->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			NrOfAliveBots++;
		}
	} 

	float MaxBotCount = 10.f;

	if (DifficutlyCurve)
	{
		MaxBotCount =  DifficutlyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}
	if (NrOfAliveBots >= MaxBotCount)
	{
		UE_LOG(LogTemp,Error,TEXT("NrOfAliveBots >= MaxBotCount"));
		return;
	}
	
	UEnvQueryInstanceBlueprintWrapper* QueryInstance =  UEnvQueryManager::RunEQSQuery(this,SpawnBotQuery,this,EEnvQueryRunMode::RandomBest5Pct,nullptr);
	if (QueryInstance)
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this,&ASGameModeBase::OnQueryCompleted);
	}
}

//Conment : If Player died, this funtion work.
//Funtion : Respawn Player
void ASGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();
		RestartPlayer(Controller);
	}
} 

// Conment: Spawn AI Bot after EQS Query return Success
// Function: Spawn AI Bot
void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{ 
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp,Error,TEXT("Spawn bot EQS Query Failed!!!"));
		return;
	}

	TArray<FVector> Location  = QueryInstance->GetResultsAsLocations();

	if (Location.IsValidIndex(0))
	{
		if (MonsterTable)
		{
			TArray<FMonsterInfoRow*> Rows; 
			MonsterTable->GetAllRows("",Rows);

			int32 RandomIndex = FMath::RandRange(0,Rows.Num()-1);
			FMonsterInfoRow* SelectedRow = Rows[RandomIndex];

			UAssetManager* Manager = UAssetManager::GetIfInitialized();
			if (Manager)
			{
				TArray<FName> Bundles;
				FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this,
					&ASGameModeBase::OnMonsterLoaded,
					SelectedRow->MonsterID,
					Location[0]);
				
				Manager->LoadPrimaryAsset(SelectedRow->MonsterID,Bundles,Delegate);
			}
		}
	}
}

void ASGameModeBase::OnPowerupSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp,Error,TEXT("Spawn PowerUp EQS Query Failed!!!"));
		return;
	}

	TArray<FVector> Location  = QueryInstance->GetResultsAsLocations();
	if (Location.IsValidIndex(0))
	{
		int32 RandomClassIdx = FMath::RandRange(0,PowerupClasses.Num()-1);
		TSubclassOf<AActor> RandomClass = PowerupClasses[RandomClassIdx];

		GetWorld()->SpawnActor<AActor>(RandomClass,Location[0],FRotator::ZeroRotator);
	}
}

void ASGameModeBase::OnMonsterLoaded(FPrimaryAssetId LoadedID, FVector SpawnLocation)
{
	LogOnScreen(this, "Finished loading.", FColor::Green);
	
	UAssetManager* Manager = UAssetManager::GetIfValid();
	if (Manager)
	{
		USMonsterData* MonsterData = Cast<USMonsterData>(Manager->GetPrimaryAssetObject(LoadedID));
		if (MonsterData)
		{
			AActor* NewBot = GetWorld()->SpawnActor<AActor>(MonsterData->MonsterClass, SpawnLocation, FRotator::ZeroRotator);
			if (NewBot)
			{
				LogOnScreen(this, FString::Printf(TEXT("Spawned enemy: %s (%s)"), *GetNameSafe(NewBot), *GetNameSafe(MonsterData)));

				// Grant special actions, buffs etc.
				USActionComponent* ActionComp = Cast<USActionComponent>(NewBot->GetComponentByClass(USActionComponent::StaticClass()));
				if (ActionComp)
				{
					for (TSubclassOf<USAction> ActionClass : MonsterData->Actions)
					{
						ActionComp->AddAction(NewBot, ActionClass);
					}
				}
			}
		}
	}
}

// SaveGame
void ASGameModeBase::WriteSaveGame()
{
	for (int32 i = 0; i < GameState->PlayerArray.Num(); i++)
	{
		ASPlayerState* PS = Cast<ASPlayerState>(GameState->PlayerArray[i]);
		if (PS)
		{
			PS->SavePlayerState(CurrentSaveGame);
			break;
		}
	}

	CurrentSaveGame->SavedActors.Empty();

	// Save all the actor that we can interact.
	for (FActorIterator It(GetWorld());	It;	++It)
	{
		AActor* Actor = *It;
		if (!Actor->Implements<USGameplayInterface>())
		{
			continue;
		}

		FActorSaveData SaveData;
		SaveData.ActorName = Actor->GetName();
		SaveData.Transform = Actor->GetActorTransform();
		
		FMemoryWriter MemWriter(SaveData.ByteData);
		FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
		Ar.ArIsSaveGame = true;
		Actor->Serialize(Ar);
		
		CurrentSaveGame->SavedActors.Add(SaveData);
	}
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame,SlotName,0);
}

void ASGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName,0))
	{
		CurrentSaveGame =  Cast<USSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName,0));
		if (CurrentSaveGame == nullptr)
		{			
			UE_LOG(LogTemp,Warning, TEXT("Failed to load the SaveGame data"));
			return;
		}

		// Find the Actor we saved, and set its transform
		for (FActorIterator It(GetWorld());	It;	++It)
		{
			AActor* Actor = *It;
			if (!Actor->Implements<USGameplayInterface>())
			{
				continue;
			}

			for (FActorSaveData ActorData : CurrentSaveGame->SavedActors)
			{
				if (ActorData.ActorName == Actor->GetName())
				{
					Actor->SetActorTransform(ActorData.Transform);

					FMemoryReader MemReader(ActorData.ByteData);

					FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
					Ar.ArIsSaveGame = true;
					Actor->Serialize(Ar);

					ISGameplayInterface::Execute_OnActorLoaded(Actor);
					break;
				}
			}
		}
	}else
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::CreateSaveGameObject(USSaveGame::StaticClass()));

		UE_LOG(LogTemp, Log, TEXT("Created New SaveGame Data."));
	}
}

void ASGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	ASPlayerState* PS = NewPlayer->GetPlayerState<ASPlayerState>();
	if (PS)
	{
		PS->LoadPlayerState(CurrentSaveGame);
		UE_LOG(LogTemp,Log,TEXT("Loading Player State"));
	}
}


