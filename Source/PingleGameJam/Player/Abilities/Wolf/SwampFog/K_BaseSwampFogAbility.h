
#pragma once

#include "CoreMinimal.h"
#include "PingleGameJam/Player/Abilities/K_BaseAbility.h"
#include "K_BaseSwampFogAbility.generated.h"


UCLASS(Blueprintable)
class UK_BaseSwampFogAbility : public UK_BaseAbility
{
	GENERATED_BODY()
	
public:
	UK_BaseSwampFogAbility();
	
	virtual bool ActivateAbility();
	virtual bool StopAbility();
	virtual bool CanActivateAbility();
};
