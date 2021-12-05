#pragma once

#include "CoreMinimal.h"
#include "K_BaseCharacterAnimInstance.h"
#include "Abilities/K_BaseAbility.h"
#include "GameFramework/Character.h"
#include "PingleGameJam/K_Support.h"
#include "K_BaseCharacter.generated.h"


DECLARE_EVENT(AK_BaseCharacter, FK_EventOnCharacterDied)


UCLASS(Blueprintable)
class AK_BaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	FK_EventOnCharacterDied EventOnCharacterDied;

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

private:
    UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* MainMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

public:
	AK_BaseCharacter();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable, Category = "CharacterMethods")
	bool CanActivateAbility(UK_BaseAbility* Ability);
	UFUNCTION(BlueprintCallable, Category = "CharacterMethods")
	UK_BaseAbility* GetAbilityByIndex(int32 AbilityIndex) const;
	UFUNCTION(BlueprintCallable, Category = "CharacterMethods")
	float GetHealth() const { return Health.GetData(); }

	UFUNCTION(Server, Reliable)
	void Server_ActivateAbility(int32 AbilityIndex, FVector Location, AK_BaseCharacter* Target);

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
};
