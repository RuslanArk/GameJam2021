
#include "K_BaseClawAttackAbility.h"

#include "Components/SphereComponent.h"
#include "PingleGameJam/Player/K_BaseCharacter.h"


UK_BaseClawAttackAbility::UK_BaseClawAttackAbility()
{
	
}

bool UK_BaseClawAttackAbility::ActivateAbility()
{
	if (Super::ActivateAbility())
	{
		
	}
	
	return false;
}

bool UK_BaseClawAttackAbility::StopAbility()
{
	return false;
}

bool UK_BaseClawAttackAbility::CanActivateAbility()
{
	return CurrentCooldown <= 0;
}

void UK_BaseClawAttackAbility::Init()
{
	AbilityCollision->OnComponentBeginOverlap.AddDynamic(this, &UK_BaseClawAttackAbility::OnClawBeginOverlap);
}

void UK_BaseClawAttackAbility::OnClawBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AK_BaseCharacter* OverlappedActor = Cast<AK_BaseCharacter>(OtherActor);
		if (OverlappedActor)
		{
			OverlappedActor->TakeDamage(GetDamageAmount(), {}, MyOwner->GetController(), MyOwner);
		}
	}
}
