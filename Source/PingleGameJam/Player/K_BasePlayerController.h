#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "K_BasePlayerController.generated.h"


UCLASS(Blueprintable)
class AK_BasePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AK_BasePlayerController();

protected:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;

	UFUNCTION()
	void OnUseActionPressed();
	UFUNCTION()
	void OnUseActionReleased();

	UFUNCTION(Server, Reliable)
	void Server_UpdateCursorLocation(FVector Location);

private:
	FVector Server_MouseCursor;
};
