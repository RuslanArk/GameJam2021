
#include "K_BaseIntimidationAbility.h"


UK_BaseIntimidationAbility::UK_BaseIntimidationAbility()
{
	
}

bool UK_BaseIntimidationAbility::ActivateAbility()
{
	if (CanActivateAbility() && Super::ActivateAbility())
	{
		ActivateCooldown();
		Client_ActivateAbility();

		// add cast logic here
		
		StopAbility();
	}
	
	return false;
}

bool UK_BaseIntimidationAbility::StopAbility()
{
	return false;
}

void UK_BaseIntimidationAbility::Client_ActivateAbility_Implementation()
{
	// Play animation here
	// Play sound here
}
