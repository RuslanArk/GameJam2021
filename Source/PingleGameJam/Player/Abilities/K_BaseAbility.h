
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "K_BaseAbility.generated.h"

class USphereComponent;

class AK_BaseCharacter;

UCLASS(Blueprintable)
class UK_BaseAbility : public UActorComponent
{
	GENERATED_BODY()

protected:
	FTimerHandle CooldownTimer;
	
	UPROPERTY(Replicated)
	AK_BaseCharacter* MyOwner = nullptr;
	
	UPROPERTY(Replicated)
	float CurrentCooldown = 0;
	UPROPERTY(Replicated)
	bool IsActivated = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "Ability_Setup")
	float MaxCooldown = 5;
	UPROPERTY(EditDefaultsOnly, Category = "Ability_Setup")
	float CooldownTickRate = 0.1;
	UPROPERTY(EditDefaultsOnly, Category = "Ability_Setup")
	float DamageAmount = 10.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Ability_Setup")
	FString AbilityName = "AbilityName";
	UPROPERTY(EditDefaultsOnly, Category = "Ability_Setup")
	UTexture2D* TextureIcon = nullptr;

public:
	UK_BaseAbility();

	virtual void Init(AK_BaseCharacter* Owner);
	
	virtual bool ActivateAbility();
	virtual bool StopAbility();
	virtual bool CanActivateAbility();

	void RestartCooldownIfIsActive(); // For Wolf ability (Intimidation)

	UFUNCTION(BlueprintCallable, Category = "Ability_Data")
	float GetCurrentCooldown() const { return CurrentCooldown; }
	UFUNCTION(BlueprintCallable, Category = "Ability_Data")
	float GetIsActive() const { return IsActivated; }
	
	UFUNCTION(BlueprintCallable, Category = "Ability_Data")
	float GetMaxCooldown() const { return MaxCooldown; }
	UFUNCTION(BlueprintCallable, Category = "Ability_Data")
	FString GetAbilityName() const { return AbilityName; }
	UFUNCTION(BlueprintCallable, Category = "Ability_Data")
	UTexture2D* GetTextureIcon() const { return TextureIcon; }
	
	UFUNCTION(BlueprintCallable, Category = "Attack")
	float GetDamageAmount() { return DamageAmount; }

	void InitAnimations();

protected:
	void ActivateCooldown();
	void ZeroedCooldown();

	void Tick_Cooldown();
	
	virtual void OnAbilityActivated();
	virtual void OnAbilityDeactivated();
	
};
