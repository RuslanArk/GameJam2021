
#pragma once

#include "CoreMinimal.h"
#include "PingleGameJam/Player/Abilities/K_BaseAbility.h"
#include "K_BaseClawAttackAbility.generated.h"


UCLASS(Blueprintable)
class UK_BaseClawAttackAbility : public UK_BaseAbility
{
	GENERATED_BODY()
	
public:
	UK_BaseClawAttackAbility();
	
	virtual bool ActivateAbility();
	virtual bool StopAbility();
	virtual bool CanActivateAbility();
};
