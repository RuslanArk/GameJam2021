
#pragma once

#include "CoreMinimal.h"
#include "PingleGameJam/Player/Abilities/K_BaseAbility.h"
#include "K_BaseIntimidationAbility.generated.h"


UCLASS(Blueprintable)
class UK_BaseIntimidationAbility : public UK_BaseAbility
{
	GENERATED_BODY()
	
public:
	UK_BaseIntimidationAbility();
	
	virtual bool ActivateAbility();
	virtual bool StopAbility();
	virtual bool CanActivateAbility();
};
