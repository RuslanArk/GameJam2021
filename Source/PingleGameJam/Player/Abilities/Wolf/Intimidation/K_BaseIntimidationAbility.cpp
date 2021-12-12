
#include "K_BaseIntimidationAbility.h"

#include "DrawDebugHelpers.h"
#include "PingleGameJam/Player/K_BaseCharacter.h"
#include "Kismet/KismetSystemLibrary.h"


bool UK_BaseIntimidationAbility::ActivateAbility()
{
	if (CanActivateAbility() && Super::ActivateAbility())
	{
		ActivateCooldown();

		// Setup sphere collision
		TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
		traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
		TArray<AActor*> ignoreActors;
		ignoreActors.Init(MyOwner, 1);
		UClass* seekClass = AK_BaseCharacter::StaticClass();

		// Find actors in radius
		TArray<AActor*> DetectedActors;
		UKismetSystemLibrary::SphereOverlapActors(GetWorld(), MyOwner->GetActorLocation(), CastDistance, traceObjectTypes, seekClass, ignoreActors, DetectedActors);		
		DrawDebugSphere(GetWorld(), MyOwner->GetActorLocation(), CastDistance, 32, FColor::Purple, true, 2.0f);

		for (auto DetectedActor : DetectedActors)
		{
			if (AK_BaseCharacter* CastedActor = Cast<AK_BaseCharacter>(DetectedActor))
			{
				// Set effect
				CastedActor->IntimidationEffect = EffectTime;

				// Reset cooldown for abilities
				for (int i = 0; i < 4; i++)
				{
					if (auto AbilityOfCharacter = CastedActor->GetAbilityByIndex(i))
					{
						AbilityOfCharacter->RestartCooldownIfIsActive();
					}
				}
			}
		}
		
		//TODO: Play animation here (add sound to animation)
		
		StopAbility(); // finish ability
	}
	
	return false;
}
