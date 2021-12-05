
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "K_BaseAbility.generated.h"


UCLASS(Blueprintable)
class UK_BaseAbility : public UObject
{
	GENERATED_BODY()

protected:
	FTimerHandle CooldownTimer;
	
	UPROPERTY(Replicated)
	float CurrentCooldown = 0;
	UPROPERTY(Replicated)
	bool IsActive = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "Ability_Setup")
	float MaxCooldown = 5;
	UPROPERTY(EditDefaultsOnly, Category = "Ability_Setup")
	float CooldownTickRate = 0.1;
	
	UPROPERTY(EditDefaultsOnly, Category = "Ability_Setup")
	FString AbilityName = "AbilityName";
	UPROPERTY(EditDefaultsOnly, Category = "Ability_Setup")
	UTexture2D* TextureIcon = nullptr;

public:
	UK_BaseAbility();

	virtual void Init();
	
	virtual bool ActivateAbility();
	virtual bool StopAbility();
	virtual bool CanActivateAbility();

	UFUNCTION(BlueprintCallable, Category = "Ability_Data")
	float GetCurrentCooldown() const { return CurrentCooldown; }
	UFUNCTION(BlueprintCallable, Category = "Ability_Data")
	float GetIsActive() const { return IsActive; }
	
	UFUNCTION(BlueprintCallable, Category = "Ability_Data")
	float GetMaxCooldown() const { return MaxCooldown; }
	UFUNCTION(BlueprintCallable, Category = "Ability_Data")
	FString GetAbilityName() const { return AbilityName; }
	UFUNCTION(BlueprintCallable, Category = "Ability_Data")
	UTexture2D* GetTextureIcon() const { return TextureIcon; }

protected:
	void ActivateCooldown();
	void ZeroedCooldown();

	void Tick_Cooldown();
};
