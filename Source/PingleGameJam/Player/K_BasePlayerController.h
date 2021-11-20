#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "K_BasePlayerController.generated.h"


UCLASS(Blueprintable)
class AK_BasePlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	uint32 bMoveToMouseCursor : 1;

public:
	AK_BasePlayerController();

protected:
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

	void MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location);

	void OnSetDestinationPressed();
	void OnSetDestinationReleased();
};


