// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include "EngineUtils.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "AI/SAICharacter.h"
#include "Assignment5/SPlayerState.h"
#include "EnvironmentQuery/EnvQueryManager.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);


ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval = 2.f;
	Credit = 0;
	CreditsPerKill = 10.f;
	PlayerStateClass = ASPlayerState::StaticClass();
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
	// this is for Debug
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

// Conment : If Player died, this funtion work.
// Funtion : Respawn Player
void ASGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();
		RestartPlayer(Controller);
	}
} 

// Conment  : Spawn AI Bot after EQS Query return Success
// Function : Spawn AI Bot
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
		GetWorld()->SpawnActor<AActor>(MinionClass,Location[0],FRotator::ZeroRotator);
	}
}

void ASGameModeBase::OnPowerupSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp,Error,TEXT("Spawn bot EQS Query Failed!!!"));
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




