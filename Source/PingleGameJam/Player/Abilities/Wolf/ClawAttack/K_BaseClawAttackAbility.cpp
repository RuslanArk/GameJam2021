
#include "K_BaseClawAttackAbility.h"


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
