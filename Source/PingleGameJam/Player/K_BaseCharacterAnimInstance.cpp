
#include "K_BaseCharacterAnimInstance.h"
#include "K_BaseCharacter.h"
#include "GameFramework/PawnMovementComponent.h"


//TODO: example
/* 
void UAnimNotify_RecoilImpulse::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (const auto Weapon = Cast<AShooterWeapon>(MeshComp->GetOwner()))
	{
		if (const auto Blueprint = Cast<UBaseCharacterAnimInstance>(Weapon->GetPawnOwner()->GetMesh()->GetAnimInstance()))
		{
			Blueprint->AddRecoilImpulse(Impulse);
		}
	}	
}
*/

UK_BaseCharacterAnimInstance::UK_BaseCharacterAnimInstance()
{
	ForwardSpeed = 0;
	RightSpeed = 0;

	IsInAir = false;
	IsIdle = false;
}

void UK_BaseCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds) {
	if (const auto Pawn = TryGetPawnOwner())
	{
		// Move
		const auto MaxSpeed = Pawn->GetMovementComponent()->GetMaxSpeed();
		if (MaxSpeed > 0)
		{
			const auto Velocity = Pawn->GetVelocity() / MaxSpeed;
			ForwardSpeed = Pawn->GetActorForwardVector() | Velocity;
			RightSpeed = Pawn->GetActorRightVector() | Velocity;
		}
		else
		{
			ForwardSpeed = 0;
			RightSpeed = 0;
		}

		// Bool
		IsInAir = Pawn->GetMovementComponent()->IsFalling();
		IsIdle = ForwardSpeed == 0 && RightSpeed == 0 && !IsInAir;
	}
}

void UK_BaseCharacterAnimInstance::StartAttackAnimation()
{
	if (AttackMontage && !Montage_IsPlaying(AttackMontage))
	{
		Montage_Play(AttackMontage, 1.0, EMontagePlayReturnType::MontageLength, 0.0f);
	}	
}
