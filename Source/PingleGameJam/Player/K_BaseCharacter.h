#pragma once

#include "CoreMinimal.h"
#include "K_BaseCharacterAnimInstance.h"
#include "GameFramework/Character.h"
#include "PingleGameJam/K_Support.h"
#include "K_BaseCharacter.generated.h"


USTRUCT(BlueprintType)
struct FK_CharacterData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterData")
	FK_FloatParameter Health = FK_FloatParameter(100);
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterData")
	FK_FloatParameter Stamina = FK_FloatParameter(100);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterSetup")
	float MovementSpeedModificator = 1.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterSetup")
	float TurnRateModificator = 1.0f;
};


DECLARE_EVENT(AK_BaseCharacter, FK_EventOnCharacterDied)


UCLASS(Blueprintable)
class AK_BaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	FK_EventOnCharacterDied EventOnCharacterDied;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MainCharacterData")
	FK_CharacterData CharacterData;

	UPROPERTY(Replicated, ReplicatedUsing = OnBodyRotationReplicated)
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
	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	void MoveTop(float Value);
	void MoveRight(float Value);
	
	void TurnRight(float Rate); // use this function for update rotation to target rotation on server side

	virtual void OnCharacterDied();

	void SetNewBodyRotation(float& NewBodyRotation);
	
	UK_BaseCharacterAnimInstance* GetAnimInstance() const { return Cast<UK_BaseCharacterAnimInstance>(GetMesh()->GetAnimInstance()); }
	class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UFUNCTION()
	virtual void OnHealthChanged(float OldHealth, float NewHealth);
	
	UFUNCTION()
	void OnBodyRotationReplicated(float& OldParameter);
	
	UFUNCTION(BlueprintCallable, Category = "CharacterParameters")
	float GetHealth() const { return CharacterData.Health.GetData(); }
	UFUNCTION(BlueprintCallable, Category = "CharacterParameters")
	float GetStamina() const { return CharacterData.Stamina.GetData(); }
};

