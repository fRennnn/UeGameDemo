// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/SBTTask_RangeAttack.h"

#include "AIController.h"
#include "SAttributeComponent.h"
#include "AI/SAICharacter.h"
#include "AI/TowerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type USBTTask_RangeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
    ATowerCharacter* MyCharacter = Cast<ATowerCharacter>(MyController->GetPawn());
    
    if (!ensure(MyCharacter))
    {
        UE_LOG(LogTemp, Error, TEXT("MyCharacter : failed"));
        return EBTNodeResult::Failed;
    }
    
    if (!ensure(MyController))
    {
        UE_LOG(LogTemp, Error, TEXT("MyController failed"));
        return EBTNodeResult::Failed;
    }

    // Check ProjectileClass is valid or not
    if (!ProjectileClass)
    {
        UE_LOG(LogTemp, Error, TEXT("ProjectileClass is null! Please set it in Blueprint or C++"));
        return EBTNodeResult::Failed;
    }

    ACharacter* MyPawn = Cast<ACharacter>(MyController->GetPawn());
    if (!MyPawn)
    {
        UE_LOG(LogTemp, Error, TEXT("MyPawn : failed"));
        return EBTNodeResult::Failed;
    }
    
    // 检查是否有 Muzzle_01 插槽
    FVector MuzzleLocation;
    if (MyPawn->GetMesh()->DoesSocketExist("Muzzle_01"))
    {
        MuzzleLocation = MyPawn->GetMesh()->GetSocketLocation("Muzzle_01");
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Muzzle_01 socket not found, using actor location"));
        MuzzleLocation = MyPawn->GetActorLocation();
        // 稍微向前偏移避免在角色内部生成
        MuzzleLocation += MyPawn->GetActorForwardVector() * 100.0f;
    }


    // Find the Target that AI want to shoot
    AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
    if (!TargetActor)
    {
        //UE_LOG(LogTemp, Error, TEXT("TargetActor : failed"));
        return EBTNodeResult::Failed;
    }

    if (!USAttributeComponent::IsActorAlive(TargetActor))
    {
        //UE_LOG(LogTemp, Error, TEXT("This Actor is Dead"));
        return EBTNodeResult::Failed;
    } 

    // 执行攻击动画
    MyCharacter->PrimaryAttack();
    
    // 计算方向和旋转
    FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
    Direction.Normalize(); // 标准化方向向量
    FRotator Rotator = Direction.Rotation();

    // Set Spawning Parameters
    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    Params.Owner = MyPawn; // 设置拥有者
    Params.Instigator = MyPawn; // 设置发起者
    
    // 获取世界对象
    UWorld* World = MyController->GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("World is null"));
        return EBTNodeResult::Failed;
    }

    // 生成投射物
    AActor* NewProj = World->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, Rotator, Params);
    
    if (!NewProj)
    {
        UE_LOG(LogTemp, Error, TEXT("SpawnActor failed - ProjectileClass: %s, Location: %s, Rotation: %s"), 
               ProjectileClass ? *ProjectileClass->GetName() : TEXT("NULL"),
               *MuzzleLocation.ToString(),
               *Rotator.ToString());
        return EBTNodeResult::Failed;
    }
    
    /*UE_LOG(LogTemp, Log, TEXT("Successfully spawned projectile: %s at location: %s"), 
           *NewProj->GetName(), *MuzzleLocation.ToString());*/
    
    return EBTNodeResult::Succeeded;
}
