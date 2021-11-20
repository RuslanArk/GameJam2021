#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "K_BaseCharacterAnimInstance.generated.h"


//TODO: example
/* 
UCLASS(const, hidecategories = Object, collapsecategories, meta = (DisplayName = "RecoilImpulse"))
class FLEAMADNESS_API UAnimNotify_RecoilImpulse : public UAnimNotify {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Impulse = 1;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
*/


UCLASS(Blueprintable)
class UK_BaseCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 IsInAir:1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 IsIdle:1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FullBodyBlend;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ForwardSpeed = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RightSpeed = 0;

public:
	UK_BaseCharacterAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void StartAttackAnimation();

	bool IsAttackAnimationPlaying() const;
};
