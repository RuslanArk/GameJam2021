
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
#include "PingleGameJam/Player/Abilities/Wolf/ClawAttack/K_BaseClawAttackAbility.h"


DEFINE_LOG_CATEGORY_STATIC(LogK_BaseCharacter, All, All);

AK_BaseCharacter::AK_BaseCharacter()
{
	bReplicates = true;
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
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
		MeleeAbility = NewObject<UK_BaseClawAttackAbility>(MainAbilityClass.Get());
		if (MeleeAbility) { MeleeAbility->Init(this); }
		MeleeAbility->InitAnimations();
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
}

void AK_BaseCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("MeleeAbility", IE_Pressed, this, &AK_BaseCharacter::ActivateMainAbility);	
	
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

	if (MainAbilityClass)
	{
		MeleeAbility = NewObject<UK_BaseClawAttackAbility>(this, MainAbilityClass);
		if (MeleeAbility)
		{
			MeleeAbility->Init(this);
		}
	}	
}

void AK_BaseCharacter::ActivateMainAbility()
{
	GetWorldTimerManager().SetTimer(TimerHandle_EffectsTick, this, &AK_BaseCharacter::EffectsTick, CONST_CHARACTER_EFFECTS_TICK_TIME, true);
}

void AK_BaseCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

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

			UE_LOG(LogK_BaseCharacter, Warning, TEXT("Damage is taken by %s"), *DamageCauser->GetName());
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
	if      (AbilityIndex == 0) { return MeleeAbility; }
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
		AddMovementInput(GetActorForwardVector(), Value * MovementSpeedModificator);
	}
}

void AK_BaseCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Value * MovementSpeedModificator);
	}
}

void AK_BaseCharacter::TurnRight(float Value)
{
	if (Value != 0.0f)
	{
		AddControllerYawInput(Value * TurnRateModificator);
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

