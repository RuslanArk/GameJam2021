
#include "K_BaseAbility.h"
#include "Net/UnrealNetwork.h"
#include "Components/SphereComponent.h"
#include "GameFramework/PawnMovementComponent.h"

#include "PingleGameJam/Player/K_BaseCharacter.h"
#include "PingleGameJam/Player/Abilities/AnimNotifies/K_ActivateAbilityAnimNotify.h"
#include "PingleGameJam/Player/Abilities/AnimNotifies/K_EndAbilityAnimNotify.h"


UK_BaseAbility::UK_BaseAbility()
{
	AbilityCollision = CreateDefaultSubobject<USphereComponent>(TEXT("AbilityCollisionComp"));
	AbilityCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AbilityCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	AbilityCollision->InitSphereRadius(70.0f);
}

void UK_BaseAbility::Init(AK_BaseCharacter* Owner)
{
	MyOwner = Owner;
	UObject::GetWorld()->GetTimerManager().SetTimer(CooldownTimer, this, &UK_BaseAbility::Tick_Cooldown, CooldownTickRate, true, 0.1f);

	AbilityCollision->AttachToComponent(MyOwner->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, MeleeSocketname);
	InitAnimations();
}

void UK_BaseAbility::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UK_BaseAbility, CurrentCooldown);
	DOREPLIFETIME(UK_BaseAbility, IsActive);
}

bool UK_BaseAbility::ActivateAbility()
{
	if (!IsActive)
	{
		IsActive = true;
		PlayMontage(AbilityMontage);
		return true;
	}
	
	return false;
}

bool UK_BaseAbility::StopAbility()
{
	if (IsActive)
	{
		IsActive = false;
		return true;
	}
	
	return false;
}

bool UK_BaseAbility::CanActivateAbility()
{
	return MyOwner && CurrentCooldown <= 0 && !IsActive;
}

void UK_BaseAbility::RestartCooldownIfIsActive()
{
	if (IsActive)
	{
		ZeroedCooldown();
		ActivateCooldown();
	}
}

void UK_BaseAbility::ActivateCooldown()
{
	if (!CurrentCooldown)
	{
		CurrentCooldown = MaxCooldown;
		GetWorld()->GetTimerManager().UnPauseTimer(CooldownTimer);
	}
}

void UK_BaseAbility::ZeroedCooldown()
{
	CurrentCooldown = 0;
	GetWorld()->GetTimerManager().PauseTimer(CooldownTimer);
}

void UK_BaseAbility::Tick_Cooldown()
{
	CurrentCooldown = FMath::Max(0.f, CurrentCooldown - CooldownTickRate);
	if (!CurrentCooldown)
	{
		GetWorld()->GetTimerManager().PauseTimer(CooldownTimer);
	}
}

void UK_BaseAbility::PlayMontage(UAnimMontage* MontageToPlay)
{
	if (!MyOwner) return;
	MyOwner->PlayAnimMontage(MontageToPlay);	
}

void UK_BaseAbility::InitAnimations()
{
	if (!AbilityMontage) return;
	const TArray<FAnimNotifyEvent> NotifyEvents = AbilityMontage->Notifies;

	for (FAnimNotifyEvent NotifyEvent : NotifyEvents)
	{
		auto AbilityActivatedNotify = Cast<UK_ActivateAbilityAnimNotify>(NotifyEvent.Notify);
		if (AbilityActivatedNotify)
		{
			AbilityActivatedNotify->OnNotified.AddUObject(this, &UK_BaseAbility::OnAbilityActivated);
			continue;
		}

		auto AbilityDeactivatedNotify = Cast<UK_EndAbilityAnimNotify>(NotifyEvent.Notify);
		if (AbilityDeactivatedNotify)
		{
			AbilityDeactivatedNotify->OnNotified.AddUObject(this, &UK_BaseAbility::OnAbilityDeactivated);
		}
	}
}

void UK_BaseAbility::OnAbilityActivated()
{
	MyOwner->GetMovementComponent()->StopMovementImmediately();	
	AbilityCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AbilityCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);	
}

void UK_BaseAbility::OnAbilityDeactivated()
{
	//MyOwner->GetMovementComponent()->Activate(true);
	AbilityCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AbilityCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
}
