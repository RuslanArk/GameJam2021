#pragma once

#include "CoreMinimal.h"
#include "K_BaseCharacterAnimInstance.h"
#include "GameFramework/Character.h"
#include "K_BaseCharacter.generated.h"


USTRUCT(BlueprintType)
struct FK_CharacterData
{
	GENERATED_BODY()

public:
	// GENERAL PARAMETERS
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterData")
	float Health = 100;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterData")
	float Stamina = 100;
	

	// MOVEMENT PARAMETERS
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterData")
	float MovementSpeedModificator = 1.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterData")
	float TurnRateModificator = 1.0f;
};


UCLASS(Blueprintable)
class AK_BaseCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MainCharacterData")
	FK_CharacterData BaseData;

	UPROPERTY(Replicated)
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

	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	void MoveTop(float Value);
	void MoveRight(float Value);
	
	void TurnAtRate(float Rate);
	
	UK_BaseCharacterAnimInstance* GetAnimInstance() const { return Cast<UK_BaseCharacterAnimInstance>(GetMesh()->GetAnimInstance()); }
	class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }


	// MODIFIED PARAMETERS
	
	// Health
	float GetHealth() const { return BaseData.Health; }
	void SetHealth(const float NewHealth) { BaseData.Health = NewHealth; }
	void AddHealth(const float AddCountHealth) { SetHealth(GetHealth() + AddCountHealth); }
	void SubtractHealth(const float SubtractCountHealth) { SetHealth(GetHealth() + SubtractCountHealth); }
	// Stamina
	float GetStamina() const { return BaseData.Stamina; }
	void SetStamina(const float NewStamina) { BaseData.Stamina = NewStamina; }
	void AddStamina(const float AddCountStamina) { SetStamina(GetStamina() + AddCountStamina); }
	void SubtractStamina(const float SubtractCountStamina) { SetStamina(GetStamina() + SubtractCountStamina); }
};

