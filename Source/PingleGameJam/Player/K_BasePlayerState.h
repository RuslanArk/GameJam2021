
#pragma once
#include "CoreMinimal.h"
#include "K_BaseCharacter.h"
#include "GameFramework/PlayerState.h"
#include "K_BasePlayerState.generated.h"


UCLASS(Blueprintable)
class AK_BasePlayerState : public APlayerState
{
	GENERATED_BODY()

protected:
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_LinkOnCharacter)
	AK_BaseCharacter* LinkOnCharacter = nullptr;

public:
	AK_BasePlayerState();
	
	AK_BaseCharacter* GetCharacter() const { return LinkOnCharacter; }
	void SetCharacter(AK_BaseCharacter* NewCharacterLink) { LinkOnCharacter = NewCharacterLink; }

protected:
	UFUNCTION()
	void OnRep_LinkOnCharacter(AK_BaseCharacter* OldData);
};
