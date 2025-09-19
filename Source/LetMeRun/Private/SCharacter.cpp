// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SAttributeComponent.h"
#include "SInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GAS/SActionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

class UEnhancedInputLocalPlayerSubsystem;
// Sets default values
ASCharacter::ASCharacter()
{
	
	//UE_LOG(LogTemp, Display, TEXT("ASCharacter::ASCharacter() 114514"));
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = true;
	
	// Camera Setting
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = true;
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->bUsePawnControlRotation = false;

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");
	AttributeComp   = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
	ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");
	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate
}

void ASCharacter::HealSelf(float AMount)
{
	AttributeComp->ApplyHealthChange(this,AMount);
}

void ASCharacter::HurtSelf(float AMount)
{
	AttributeComp->ApplyHealthChange(this,-AMount);
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();

	//EnhancedInput Subsystem
	APlayerController* PlayerControl = Cast<APlayerController>(Controller);
	if (PlayerControl)
	{
		//UE_LOG(LogTemp, Display, TEXT("ASCharacter::BeginPlay()"));
		UEnhancedInputLocalPlayerSubsystem* LocalSS = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerControl->GetLocalPlayer());
		if (LocalSS)
		{
			LocalSS->AddMappingContext(PlayerInputMapping,0);
		}
	} 
	
}

FVector ASCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent* PlayerInputC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	// Bind the player input 
	if (PlayerInputC)
	{
		PlayerInputC->BindAction(PlayerMove,ETriggerEvent::Triggered,this,&ASCharacter::Move);
		PlayerInputC->BindAction(PlayerLook,ETriggerEvent::Triggered,this,&ASCharacter::Look);
		PlayerInputC->BindAction(PlayerJump,ETriggerEvent::Started,this,&ASCharacter::Jump);
		PlayerInputC->BindAction(PlayerJump,ETriggerEvent::Completed,this,&ASCharacter::StopJumping);
		PlayerInputC->BindAction(PlayerShoot,ETriggerEvent::Started,this,&ASCharacter::PrimaryAttack);
		PlayerInputC->BindAction(PlayerInteraction,ETriggerEvent::Started,this,&ASCharacter::PrimaryInteract);
		PlayerInputC->BindAction(PlayerSkill,ETriggerEvent::Started,this,&ASCharacter::PrimaryUltimate);
		PlayerInputC->BindAction(PlayerSprint,ETriggerEvent::Started,this,&ASCharacter::SprintStart);
		PlayerInputC->BindAction(PlayerSprint,ETriggerEvent::Completed,this,&ASCharacter::SprintEnd);
	}else
	{
		UE_LOG(LogTemp,Error,TEXT("ASCharacter::SetupPlayerInputComponent() Failed"));
	}
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this,&ASCharacter::OnHealthChanged);
}

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
                                  float Delta)
{
	// Damaged
	if (Delta < 0.0f)
	{
		// Rage added equal to damage received (Abs to turn into positive rage number)
		float RageDelta = FMath::Abs(Delta);
		AttributeComp->ApplyRage(InstigatorActor, RageDelta);
	}
	
	if (NewHealth <= 0.f && Delta <= 0.f)
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		DisableInput(PlayerController);

		SetLifeSpan(5.f);
	}
}

void ASCharacter::SpawnProjectile(TSubclassOf<class AActor> ClassToSpawn)
{
	if (ensureAlways(ClassToSpawn))
	{
		FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
		FVector WorldLocation,WorldDirection;
		GetScreenCenterToWorld(WorldLocation,WorldDirection);
		
		// Get the CenterPoint
		FVector TargetLocation = LineTrace(WorldLocation,WorldDirection);
		FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(HandLocation,TargetLocation);
		
		FCollisionShape Shape;
		Shape.SetSphere(20.0f);
		
		// Ignore Player
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		
		FTransform SpawnTM = FTransform(NewRotation, HandLocation);
	
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;
		GetWorld()->SpawnActor<AActor>(ClassToSpawn,SpawnTM,SpawnParams);
	}
}



void ASCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}

void ASCharacter::SprintStart()
{
	ActionComp->StartActionByName(this,"Sprint");
}

void ASCharacter::SprintEnd()
{
	ActionComp->StopActionByName(this,"Sprint");
}

void ASCharacter::PrimaryAttack()
{
	ActionComp->StartActionByName(this,"PrimaryAttack");
}

void ASCharacter::PrimaryUltimate()
{
	ActionComp->StartActionByName(this, "Blackhole");
}


bool ASCharacter::GetScreenCenterToWorld(FVector& WorldLocation, FVector& WorldDirection) const
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController)
	{
		//(LogTemp,Error,TEXT("Faild to Get PlayerController"));
		return false;
	}
	int32 ViewportSizeX = 0, ViewportSizeY = 0;
	PlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);

	FVector2D CrossHair2D(ViewportSizeX / 2.f, ViewportSizeY / 2.f);
	
	return PlayerController->DeprojectScreenPositionToWorld(
		CrossHair2D.X,
		CrossHair2D.Y,
		WorldLocation,
		WorldDirection);
}

FVector ASCharacter::LineTrace(FVector& Start, FVector& Direction)
{
	float TraceDistance = 10000.f;
	FVector TraceEnd = Start + Direction * TraceDistance;

	FHitResult Hit;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = false;
	QueryParams.bReturnPhysicalMaterial = false;
	
	// Set Collision ObjectType - WorldDynamic & WorldStatic
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	bool b_HitResult = GetWorld()->LineTraceSingleByObjectType(
		Hit,
		Start,
		TraceEnd,
		ObjectTypes,
		QueryParams);

	if (b_HitResult)
	{
		return Hit.Location;
	}
	return TraceEnd;
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get the right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ASCharacter::Look(const FInputActionValue& Value)
{
	const FVector2d F2d = Value.Get<FVector2d>();
	if (Controller)
	{
		AddControllerPitchInput(F2d.Y);
		AddControllerYawInput(F2d.X);
	}
}


inline void ASCharacter::DrawDebugLine()
{
	FVector LineStart = GetActorLocation();
	// Offset to the right of pawn
	LineStart += GetActorRightVector() * 100.0f;
	// Set line end in direction of the actor's forward
	FVector ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 100.0f);
	// Draw Actor's Direction
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirection_LineEnd, 500, FColor::Yellow, false, 0.0f, 0, 5);
	
	FVector ControllerDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 100.0f);
	// Draw 'Controller' Rotation ('PlayerController' that 'possessed' this character)
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerDirection_LineEnd, 500, FColor::Green, false, 0.0f, 0, 5);
}
