
#include "K_BaseClawAttackAbility.h"

#include "Components/SphereComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "PingleGameJam/Player/K_BaseCharacter.h"

DEFINE_LOG_CATEGORY_STATIC(LogK_MeleeAbility, All, All);

UK_BaseClawAttackAbility::UK_BaseClawAttackAbility()
{
	AbilityCollision = CreateDefaultSubobject<USphereComponent>(TEXT("AbilityCollisionComp"));
	AbilityCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AbilityCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	AbilityCollision->InitSphereRadius(70.0f);
}

bool UK_BaseClawAttackAbility::ActivateAbility()
{
	if (CanActivateAbility() && Super::ActivateAbility())
	{
		if (UK_BaseCharacterAnimInstance* AnimInstance = Cast<UK_BaseCharacterAnimInstance>(MyOwner->GetMesh()->GetAnimInstance()))
		{		
			AnimInstance->StartAttackAnimation();
			UE_LOG(LogK_MeleeAbility, Warning, TEXT("Attack animation played"));
		}
		
		StopAbility();
		return true;
	}
	
	return false;
}

void UK_BaseClawAttackAbility::Init(AK_BaseCharacter* Owner)
{
	Super::Init(Owner);
	
	AbilityCollision->AttachToComponent(MyOwner->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, MeleeSocketName);
	AbilityCollision->OnComponentBeginOverlap.AddDynamic(this, &UK_BaseClawAttackAbility::OnClawBeginOverlap);
}

void UK_BaseClawAttackAbility::OnClawBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AK_BaseCharacter* OverlappedActor = Cast<AK_BaseCharacter>(OtherActor);
		if (OverlappedActor)
		{
			UE_LOG(LogK_MeleeAbility, Warning, TEXT("Melee ability might cause damage"));
			OverlappedActor->TakeDamage(GetDamageAmount(), {}, MyOwner->GetController(), MyOwner);
		}
	}
}


void UK_BaseClawAttackAbility::OnAbilityActivated()
{
	MyOwner->GetMovementComponent()->StopMovementImmediately();	
	AbilityCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AbilityCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);	
}

void UK_BaseClawAttackAbility::OnAbilityDeactivated()
{
	//MyOwner->GetMovementComponent()->Activate(true);
	AbilityCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AbilityCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
}

