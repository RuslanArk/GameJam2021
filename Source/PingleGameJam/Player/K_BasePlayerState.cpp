
#include "K_BasePlayerState.h"
#include "Net/UnrealNetwork.h"


void AK_BasePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AK_BasePlayerState, LinkOnCharacter);
}

AK_BasePlayerState::AK_BasePlayerState()
{
	
}

void AK_BasePlayerState::OnRep_LinkOnCharacter(AK_BaseCharacter* OldData)
{
	
}
