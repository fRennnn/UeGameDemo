// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

static void LogOnScreen(UObject* WorldContext,FString Msg,FColor Color = FColor::White,float Duration = 5.f)
{
	if (!ensure(WorldContext))
	{
		UE_LOG(LogTemp,Error,TEXT("WorldContext failed"));
		return;
	}

	UWorld * WOrld = WorldContext->GetWorld();
	if (!ensure(WOrld))
	{
		UE_LOG(LogTemp,Error,TEXT("World failed"));
		return;
	}

	FString NetPrefix =  WOrld->IsNetMode(NM_Client)? "[CLIENT]" : "[SERVER]";
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1,Duration,Color,NetPrefix + Msg);
	}
}