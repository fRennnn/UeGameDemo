// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/SAIController.h"
#include "T_AIController.generated.h"
class UAIPerceptionComponent;
class UAISenseConfig_Sight;
/**
 * 
 */
UCLASS()
class LETMERUN_API AT_AIController : public ASAIController
{
	GENERATED_BODY()
public:
	AT_AIController();

	virtual void BeginPlay() override;
protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Perception")
	UAIPerceptionComponent* PerceptionComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Perception")
	UAISenseConfig_Sight* SightConfig;
	
};
