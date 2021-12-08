
#include "K_BaseWolf.h"
#include "GameFramework/CharacterMovementComponent.h"


AK_BaseWolf::AK_BaseWolf()
{
	
}

void AK_BaseWolf::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AK_BaseWolf::BeginPlay()
{
	Super::BeginPlay();

}

void AK_BaseWolf::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

float AK_BaseWolf::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                                   AActor* DamageCauser)
{	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AK_BaseWolf::OnCharacterDied()
{
	
}
