
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
	UPROPERTY()
	FTimerHandle AttackTimer;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* AbilityCollision;
	
	UPROPERTY(VisibleAnywhere)
	FName MeleeSocketName = "MeleeAttackSocket";
	
	
	UK_BaseClawAttackAbility();
	
	virtual bool ActivateAbility() override;
	virtual void Init(AK_BaseCharacter* Owner) override;

	UFUNCTION()
	void OnClawBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OverlapTick();

private:
	virtual void OnAbilityActivated() override;
	virtual void OnAbilityDeactivated() override;
};
