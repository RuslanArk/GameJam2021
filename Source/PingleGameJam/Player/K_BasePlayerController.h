#pragma once

#include "CoreMinimal.h"
#include "K_BaseCharacter.h"
#include "GameFramework/PlayerController.h"
#include "K_BasePlayerController.generated.h"

class APlayerStart;

UCLASS(Blueprintable)
class AK_BasePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AK_BasePlayerController();

	UPROPERTY(EditDefaultsOnly, Category = "PlayerStart")
	const TSubclassOf<APlayerStart> PlayerStartClass;

	UFUNCTION()
	void TriggerRespawn();

	UFUNCTION(NetMulticast, Reliable)
	void Server_RespawnPlayer();

	FTransform FindSpawnPlace();

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
	FTimerHandle RespawnTimer;
	
	
};
