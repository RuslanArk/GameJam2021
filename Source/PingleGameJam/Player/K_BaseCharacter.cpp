
#include "K_BaseCharacter.h"

#include "K_BasePlayerState.h"
#include "Camera/CameraComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Components/CapsuleComponent.h"
#include "Framework/MultiBox/ToolMenuBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"

#include "PingleGameJam/Player/Abilities/K_BaseAbility.h"


DEFINE_LOG_CATEGORY_STATIC(LogK_BaseCharacter, All, All);

AK_BaseCharacter::AK_BaseCharacter()
{
	bReplicates = true;
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	GetCharacterMovement()->SetIsReplicated(true);

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 900.f;
	CameraBoom->SetRelativeRotation(FRotator(-80.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	
	if (MainAbilityClass)
	{
		MainAbility = NewObject<UK_BaseAbility>(MainAbilityClass.Get());
		if (MainAbility) { MainAbility->Init(this); }
		MainAbility->InitAnimations();
	}

	if (Ability1Class)
	{
		Ability1 = NewObject<UK_BaseAbility>(Ability1Class.Get());
		if (Ability1) { Ability1->Init(this); }
	}

	if (Ability2Class)
	{
		Ability2 = NewObject<UK_BaseAbility>(Ability2Class.Get());
		if (Ability2) { Ability2->Init(this); }
	}
	
	if (Ability3Class)
	{
		Ability3 = NewObject<UK_BaseAbility>(Ability3Class.Get());
		if (Ability3) { Ability3->Init(this); }
	}
}

void AK_BaseCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AK_BaseCharacter, Health);
	DOREPLIFETIME(AK_BaseCharacter, BodyRotation);
}

void AK_BaseCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("ActivateMeleeAbility", IE_Pressed, this, &AK_BaseCharacter::ActivateMainAbility);	
	
	PlayerInputComponent->BindAxis("MoveTop", this, &AK_BaseCharacter::MoveTop);
	PlayerInputComponent->BindAxis("MoveRight", this, &AK_BaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis("TurnAtRate", this, &AK_BaseCharacter::TurnRight);

	PlayerInputComponent->BindAction("ActivateAbility1", IE_Pressed, this, &AK_BaseCharacter::ActionActivateAbility1);
	PlayerInputComponent->BindAction("ActivateAbility2", IE_Pressed, this, &AK_BaseCharacter::ActionActivateAbility2);
	PlayerInputComponent->BindAction("ActivateAbility3", IE_Pressed, this, &AK_BaseCharacter::ActionActivateAbility3);
}

void AK_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	EventOnCharacterDied.AddUObject(this, &AK_BaseCharacter::OnCharacterDied);
	Health.OnParameterChanged.AddDynamic(this, &AK_BaseCharacter::OnHealthChanged);

	if (MainAbility)
	{
		MainAbility->AbilityCollision->OnComponentBeginOverlap.AddDynamic(this, &AK_BaseCharacter::OnMeleeAbilitySphereBeginOverlap);
	}
}

void AK_BaseCharacter::ActivateMainAbility()
{
	if (!MainAbility) return;

	if (MainAbility->CanActivateAbility())
	{
		MainAbility->ActivateAbility();
		
	}

	GetWorldTimerManager().SetTimer(TimerHandle_EffectsTick, this, &AK_BaseCharacter::EffectsTick, CONST_CHARACTER_EFFECTS_TICK_TIME, true);
}

void AK_BaseCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateVisibilityOfWolf();
	UpdateVisibilityOfTeammates();
}

float AK_BaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                                   AActor* DamageCauser)
{
	if (EventInstigator && DamageCauser)
	{
		// Check, can we take damage here?
	
		if (DamageAmount > 0)
		{
			// Deal damage
			Health.SetData(FMath::Max<float>(Health.GetData() - DamageAmount, 0));

			UE_LOG(LogK_BaseCharacter, Warning, TEXT("Damage is taken by %s"), *GetName());
		}
	}	
	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AK_BaseCharacter::EffectsTick()
{
	if (IntimidationEffect)
	{
		IntimidationEffect = FMath::Max(IntimidationEffect - CONST_CHARACTER_EFFECTS_TICK_TIME, 0.f);
	}
}

bool AK_BaseCharacter::CanActivateAbility(UK_BaseAbility* Ability)
{
	return IsValid(Ability) && CanActivateAbilityCheck(Ability);
}

UK_BaseAbility* AK_BaseCharacter::GetAbilityByIndex(const int32 AbilityIndex) const
{
	// Shit code, but do not care about it XD
	if      (AbilityIndex == 0) { return MainAbility; }
	else if (AbilityIndex == 1) { return Ability1; }
	else if (AbilityIndex == 2) { return Ability2; }
	else if (AbilityIndex == 3) { return Ability3; }

	return nullptr;
}

void AK_BaseCharacter::SetLocalVisibility(bool IsVisibility)
{
	GetMesh()->SetVisibility(IsVisibility);
}

void AK_BaseCharacter::MoveTop(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(FVector(1, 0, 0), Value * MovementSpeedModificator);
	}
}

void AK_BaseCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(FVector(0, 1, 0), Value * MovementSpeedModificator);
	}
}

void AK_BaseCharacter::TurnRight(float Value)
{
	if (Value != 0.0f)
	{
		float NewBodyRotation = BodyRotation + Value * TurnRateModificator * GetWorld()->GetDeltaSeconds();
		NewBodyRotation = NewBodyRotation > 360 ? NewBodyRotation - 360 : NewBodyRotation < 0 ? NewBodyRotation + 360 : NewBodyRotation;
		SetNewBodyRotation(NewBodyRotation);
	}
}

void AK_BaseCharacter::OnCharacterDied()
{
	//TODO: play death animation
	if (GetMovementComponent())
	{
		GetMovementComponent()->StopMovementImmediately();		
	}
	SetActorEnableCollision(false);	
	SetLifeSpan(5.0f);

	GetWorldTimerManager().SetTimer(RespawnTimer, this, &AK_BaseCharacter::RespawnPlayer, RespawnRate);
	
	EventOnCharacterDied.Broadcast();
}

void AK_BaseCharacter::RespawnPlayer()
{
	
}

bool AK_BaseCharacter::CanActivateAbilityCheck(UK_BaseAbility* Ability)
{
	if (IsValid(Ability))
	{
		// If we need to check some special logic, we put it here
		return Ability->CanActivateAbility();
	}

	return false;
}

void AK_BaseCharacter::SetNewBodyRotation(float& NewBodyRotation)
{
	BodyRotation = NewBodyRotation;
	FRotator NewRotate = GetActorRotation();
	NewRotate.Yaw = BodyRotation;
	SetActorRotation(NewRotate);
}

void AK_BaseCharacter::ActionActivateAbility1()
{
	Server_ActivateAbility(1, FVector(), nullptr);
}

void AK_BaseCharacter::ActionActivateAbility2()
{
	Server_ActivateAbility(2, FVector(), nullptr);
}

void AK_BaseCharacter::ActionActivateAbility3()
{
	Server_ActivateAbility(3, FVector(), nullptr);
}

void AK_BaseCharacter::OnHealthChanged(float OldHealth, float NewHealth)
{
	if (NewHealth <= 0)
	{
		OnCharacterDied();
	}
}

void AK_BaseCharacter::OnRep_BodyRotation(float& OldParameter)
{
	SetNewBodyRotation(BodyRotation);
}

void AK_BaseCharacter::UpdateVisibilityOfWolf()
{
	
}

void AK_BaseCharacter::UpdateVisibilityOfTeammates()
{
	if (AGameStateBase* const MyGameState = GetWorld() != nullptr ? GetWorld()->GetGameState<AGameStateBase>() : nullptr)
	{
		for (APlayerState* PS : MyGameState->PlayerArray)
		{
			if (AK_BasePlayerState* CastedPlayerState = Cast<AK_BasePlayerState>(PS))
			{
				auto PlayerCharacter = CastedPlayerState->GetCharacter();
				if (PlayerCharacter && GetPlayerState() != CastedPlayerState) // && !CastedPlayerState->IsWolf // skip us and wolf //TODO: add check not wolf logic
				{
					if (IntimidationEffect)
					{
						//TODO: hide all teammates
						PlayerCharacter->SetLocalVisibility(false);
					}
					else
					{
						//TODO: show all teammates
						PlayerCharacter->SetLocalVisibility(true);
					}
				}
			}
		}
	}
}

void AK_BaseCharacter::Server_ActivateAbility_Implementation(int32 AbilityIndex, FVector Location, AK_BaseCharacter* Target)
{
	const auto Ability = GetAbilityByIndex(AbilityIndex);
	if (CanActivateAbility(Ability))
	{
		Ability->ActivateAbility();
	}
}

void AK_BaseCharacter::OnMeleeAbilitySphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AK_BaseCharacter* OverlappedActor = Cast<AK_BaseCharacter>(OtherActor);
		if (OverlappedActor)
		{
			OverlappedActor->TakeDamage(MainAbility->GetDamageAmount(), {}, GetController(), this);
			UE_LOG(LogK_BaseCharacter, Warning, TEXT("%s took: %d damage"), *OverlappedActor->GetName(), MainAbility->GetDamageAmount());
		}
	}
}
