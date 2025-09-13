// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, USAttributeComponent*, OwningComp, float, NewHealth, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnRageChanged, AActor*, InstigatorActor, USAttributeComponent*, OwningComp, float, NewValue, float, Delta);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LETMERUN_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USAttributeComponent();
	
	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UFUNCTION(NetMulticast, Reliable)// @FIXME: mark as unreliable once we moved the 'state' our of scharacter
	void MulticastHealthChanged(AActor* InstigatorActor, float NewHealth, float Delta);
	
	UFUNCTION(NetMulticast, Unreliable) // Just for Performance
	void MulticastRageChanged(AActor* InstigatorActor, float NewRage, float Delta);
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category="Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor,float Delta);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static USAttributeComponent* GetAttributes(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category = "Attributes", meta = (DisplayName = "IsAlive"))
	static bool IsActorAlive(AActor* Actor);
	
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnRageChanged OnRageChanged;

protected:
	// EditAnywhere - 可以在蓝图编辑器中编辑并且在关卡中每个实例都可以编辑.
	// VisibleAnywhere - 在关卡和编辑器中只读 (用于组件)
	// EditDefaultsOnly - 对于生成的实例属性是隐藏的, 只能在蓝图编辑器中编辑
	// VisibleDefaultsOnly - 对象只在蓝图编辑器中可见且只读 (不常见)
	// EditInstanceOnly - 只能编辑是实例 (例如在关卡中)
	// --
	// BlueprintReadOnly - 在蓝图编程中只读 (不影响蓝图右侧细节面板的数值调整)
	// BlueprintReadWrite - 在蓝图变成中可以读写
	// --
	// Category = "" - 只在细节面板和蓝图的上下文搜索时显示的类别
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Replicated,Category="Attributes")
	float Health;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Replicated, Category = "Attributes")
	float HealthMax;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Replicated,Category="Attributes")
	float Rage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Replicated,Category = "Attributes")
	float RageMax;

public:	

	UFUNCTION(BlueprintCallable)
	bool IsFullHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetHealthMax() const;

	UFUNCTION(BlueprintCallable)
	float GetHealth() const;

	UFUNCTION(Exec)
	float Kill(AActor* Instigator);

	UFUNCTION(BlueprintCallable)
	float GetRage() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyRage(AActor* InstigatorActor, float Delta);
};
