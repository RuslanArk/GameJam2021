
#include "K_BaseSwampFogAbility.h"


UK_BaseSwampFogAbility::UK_BaseSwampFogAbility()
{
	
}

bool UK_BaseSwampFogAbility::ActivateAbility()
{
	if (Super::ActivateAbility())
	{
		
	}
	
	return false;
}

bool UK_BaseSwampFogAbility::StopAbility()
{
	return false;
}

bool UK_BaseSwampFogAbility::CanActivateAbility()
{
	return CurrentCooldown <= 0;
}
