
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
	//UPROPERTY(EditDefaultsOnly, Category = "AbilityIntimidation_Setup")
	//float CastTime = 0.5;

public:
	UK_BaseIntimidationAbility();
	
	virtual bool ActivateAbility() override;
	virtual bool StopAbility() override;

protected:
	UFUNCTION(Reliable, Client)
	void Client_ActivateAbility();
};
