#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "K_BaseWolf.generated.h"


UCLASS(Blueprintable)
class AK_BaseWolf : public ACharacter
{
	GENERATED_BODY()

public:
	AK_BaseWolf();

	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void OnCharacterDied();
};
