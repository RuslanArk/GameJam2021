#pragma once

#include "CoreMinimal.h"
#include "K_BaseCharacterAnimInstance.h"
#include "GameFramework/Character.h"
#include "PingleGameJam/K_Support.h"
#include "K_BaseCharacter.generated.h"

class UK_BaseAbility;

DECLARE_EVENT(AK_BaseCharacter, FK_EventOnCharacterDied)


#define CONST_CHARACTER_EFFECTS_TICK_TIME 0.1f


UCLASS(Blueprintable)
class AK_BaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	FK_EventOnCharacterDied EventOnCharacterDied;

	// All Effects (The shortest way - is a timer, when 0 - is disable)
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Effects")
	float IntimidationEffect = 0; // UK_BaseIntimidationAbility - can not see teammates, lock abilities (reset abilities cooldown do once form ability).

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Abilities")
	UK_BaseAbility* MainAbility = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "Abilities")
	UK_BaseAbility* Ability1 = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "Abilities")
	UK_BaseAbility* Ability2 = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "Abilities")
	UK_BaseAbility* Ability3 = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterSetup")
	TSubclassOf<UK_BaseAbility> MainAbilityClass;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterSetup")
	TSubclassOf<UK_BaseAbility> Ability1Class;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterSetup")
	TSubclassOf<UK_BaseAbility> Ability2Class;
	UPROPERTY(EditDefaultsOnly, Category = "CharacterSetup")
	TSubclassOf<UK_BaseAbility> Ability3Class;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterSetup")
	float MovementSpeedModificator = 1.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterSetup")
	float TurnRateModificator = 1.0f;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterData")
	FK_FloatParameter Health;
	
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterData", ReplicatedUsing = OnRep_BodyRotation)
	float BodyRotation = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RespawnRate = 5.0f;

private:
    UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* MainMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	FTimerHandle TimerHandle_EffectsTick;

	FTimerHandle RespawnTimer;

public:
	AK_BaseCharacter();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void EffectsTick();
	
	UFUNCTION(BlueprintCallable, Category = "CharacterMethods")
	bool CanActivateAbility(UK_BaseAbility* Ability);
	UFUNCTION(BlueprintCallable, Category = "CharacterMethods")
	UK_BaseAbility* GetAbilityByIndex(int32 AbilityIndex) const;
	UFUNCTION(BlueprintCallable, Category = "CharacterMethods")
	float GetHealth() const { return Health.GetData(); }

	void SetLocalVisibility(bool IsVisibility);
	
	UFUNCTION(Server, Reliable)
	void Server_ActivateAbility(int32 AbilityIndex, FVector Location, AK_BaseCharacter* Target);

	void ActivateMainAbility();
	

protected:
	void MoveTop(float Value);
	void MoveRight(float Value);
	
	void TurnRight(float Rate); // use this function for update rotation to target rotation on server side

	virtual void OnCharacterDied();
	virtual bool CanActivateAbilityCheck(UK_BaseAbility* Ability);

	void SetNewBodyRotation(float& NewBodyRotation);
	
	UK_BaseCharacterAnimInstance* GetAnimInstance() const { return Cast<UK_BaseCharacterAnimInstance>(GetMesh()->GetAnimInstance()); }
	class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	
	UFUNCTION()
	virtual void OnHealthChanged(float OldHealth, float NewHealth);
	
	UFUNCTION()
	void OnRep_BodyRotation(float& OldParameter);

private:
	void RespawnPlayer();	
	
	UFUNCTION()
	void OnMeleeAbilitySphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	

private:
	void UpdateVisibilityOfWolf();
	void UpdateVisibilityOfTeammates(); // Using for UK_BaseIntimidationAbility
};
