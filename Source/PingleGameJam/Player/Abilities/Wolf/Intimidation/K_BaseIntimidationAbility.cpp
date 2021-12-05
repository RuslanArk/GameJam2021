
#include "K_BaseIntimidationAbility.h"


UK_BaseIntimidationAbility::UK_BaseIntimidationAbility()
{
	
}

bool UK_BaseIntimidationAbility::ActivateAbility()
{
	if (Super::ActivateAbility())
	{
		
	}
	
	return false;
}

bool UK_BaseIntimidationAbility::StopAbility()
{
	return false;
}

bool UK_BaseIntimidationAbility::CanActivateAbility()
{
	return CurrentCooldown <= 0;
}
