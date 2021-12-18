
#pragma once
#include "CoreMinimal.h"
#include "K_BaseCharacter.h"
#include "GameFramework/PlayerState.h"
#include "K_BasePlayerState.generated.h"


UCLASS(Blueprintable)
class AK_BasePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	bool IsWolf = false;
	bool IsDead = false;

protected:
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_LinkOnCharacter)
	AK_BaseCharacter* LinkOnCharacter = nullptr;

public:
	AK_BasePlayerState();

	void SetCharacter(AK_BaseCharacter* NewCharacterLink) { LinkOnCharacter = NewCharacterLink; }

	UFUNCTION(BlueprintCallable)
	AK_BaseCharacter* GetCharacter() const { return LinkOnCharacter; }

protected:
	UFUNCTION()
	void OnRep_LinkOnCharacter(AK_BaseCharacter* OldData);
};
