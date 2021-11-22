// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PingleGameJamGameModeBase.h"
#include "K_LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PINGLEGAMEJAM_API AK_LobbyGameMode : public APingleGameJamGameModeBase
{
	GENERATED_BODY()

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

protected:
	void StartGame();

private:
	uint32 NumberOfPlayers = 0;

	FTimerHandle ServerTravelTimer;
	
};
