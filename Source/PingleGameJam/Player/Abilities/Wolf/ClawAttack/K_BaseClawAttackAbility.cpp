
#include "K_BaseClawAttackAbility.h"
#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PingleGameJam/Player/K_BaseCharacter.h"

DEFINE_LOG_CATEGORY_STATIC(LogK_MeleeAbility, All, All);

UK_BaseClawAttackAbility::UK_BaseClawAttackAbility()
{
	AbilityCollision = CreateDefaultSubobject<USphereComponent>(TEXT("AbilityCollisionComp"));
	AbilityCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AbilityCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	AbilityCollision->InitSphereRadius(90.0f);
}

bool UK_BaseClawAttackAbility::ActivateAbility()
{
	if (CanActivateAbility() && Super::ActivateAbility())
	{
		if (UK_BaseCharacterAnimInstance* AnimInstance = Cast<UK_BaseCharacterAnimInstance>(MyOwner->GetMesh()->GetAnimInstance()))
		{		
			AnimInstance->StartAttackAnimation();

			MyOwner->Client_StartAttackAnimation();
			UE_LOG(LogK_MeleeAbility, Warning, TEXT("Attack animation played server"));
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

	GetWorld()->GetTimerManager().SetTimer(AttackTimer, this, &UK_BaseClawAttackAbility::OverlapTick, 0.05, true, 0.1f);
	GetWorld()->GetTimerManager().PauseTimer(AttackTimer);
}

void UK_BaseClawAttackAbility::OnClawBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogK_MeleeAbility, Warning, TEXT("Melee ability overlap detected"));
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

void UK_BaseClawAttackAbility::OverlapTick()
{
	DrawDebugSphere(GetWorld(), AbilityCollision->GetComponentLocation(), 20, 4, FColor::Purple, true, 2.0f);
	UE_LOG(LogK_MeleeAbility, Warning, TEXT("OverlapTick"));
	
	if (AbilityCollision)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AK_BaseCharacter::StaticClass(), FoundActors);

		for (auto OtherActor : FoundActors)
		{
			if (OtherActor && OtherActor != MyOwner && FVector::Distance(OtherActor->GetActorLocation(), MyOwner->GetActorLocation()) < 200)
			{
				UE_LOG(LogK_MeleeAbility, Warning, TEXT("Melee ability might cause damage"));
				OtherActor->TakeDamage(GetDamageAmount(), {}, MyOwner->GetController(), MyOwner);
			}
		}

		/*
		TArray<AActor*> OverlappedActors;
		AbilityCollision->GetOverlappingActors(OverlappedActors, AK_BaseCharacter::StaticClass());

		for (auto OtherActor : OverlappedActors)
		{
			if (OtherActor && OtherActor != MyOwner)
			{
				AK_BaseCharacter* OverlappedActor = Cast<AK_BaseCharacter>(OtherActor);
				if (OverlappedActor)
				{
					UE_LOG(LogK_MeleeAbility, Warning, TEXT("Melee ability might cause damage"));
					OverlappedActor->TakeDamage(GetDamageAmount(), {}, MyOwner->GetController(), MyOwner);
				}
			}
		}
		*/
	}
}

void UK_BaseClawAttackAbility::OnAbilityActivated()
{
	GetWorld()->GetTimerManager().UnPauseTimer(AttackTimer);
	
	MyOwner->GetMovementComponent()->StopMovementImmediately();	
	//AbilityCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//AbilityCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	//AbilityCollision->OnComponentBeginOverlap.AddDynamic(this, &UK_BaseClawAttackAbility::OnClawBeginOverlap);
	//DrawDebugSphere(GetWorld(), AbilityCollision->GetComponentLocation(), 20, 4, FColor::Purple, true, 2.0f);
}

void UK_BaseClawAttackAbility::OnAbilityDeactivated()
{
	GetWorld()->GetTimerManager().PauseTimer(AttackTimer);
	
	//AbilityCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//AbilityCollision->OnComponentBeginOverlap.RemoveAll(this);
	//DrawDebugSphere(GetWorld(), AbilityCollision->GetComponentLocation(), 20, 4, FColor::Green, true, 2.0f);
}

