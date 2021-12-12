
#include "K_BaseAbility.h"
#include "Net/UnrealNetwork.h"


UK_BaseAbility::UK_BaseAbility()
{
	
}

void UK_BaseAbility::Init(AK_BaseCharacter* Owner)
{
	MyOwner = Owner;
	UObject::GetWorld()->GetTimerManager().SetTimer(CooldownTimer, this, &UK_BaseAbility::Tick_Cooldown, CooldownTickRate, true, 0.1f);
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
