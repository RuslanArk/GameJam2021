
#pragma once

#include "CoreMinimal.h"
#include "PingleGameJam/Player/Abilities/K_BaseAbility.h"
#include "K_BaseIntimidationAbility.generated.h"


UCLASS(Blueprintable)
class UK_BaseIntimidationAbility : public UK_BaseAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "AbilityIntimidation_Setup")
	float CastDistance = 250;
	UPROPERTY(EditDefaultsOnly, Category = "AbilityIntimidation_Setup")
	float EffectTime = 5;

	//TODO: add animation, and play it when ability started

public:
	virtual bool ActivateAbility() override;
};
