
#pragma once

#include "CoreMinimal.h"
#include "PingleGameJam/Player/Abilities/K_BaseAbility.h"
#include "K_BaseClawAttackAbility.generated.h"

class USphereComponent;

UCLASS(Blueprintable)
class UK_BaseClawAttackAbility : public UK_BaseAbility
{
	GENERATED_BODY()
	
public:
	UK_BaseClawAttackAbility();
	
	virtual bool ActivateAbility();
	virtual bool StopAbility();
	virtual bool CanActivateAbility();

protected:
	virtual void Init();

private:
	UFUNCTION()
	void OnClawBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
};
