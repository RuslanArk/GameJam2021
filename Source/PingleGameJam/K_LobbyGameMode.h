// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PingleGameJamGameModeBase.h"
#include "Player/K_BasePlayerController.h"
#include "K_LobbyGameMode.generated.h"

class APlayerStart;

class AK_BaseCharacter;

USTRUCT(BlueprintType)
struct FPlayersRoles
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AK_BaseCharacter>> PlayerClasses;	
	TMap<int32, bool> RoleIsTaken;
};

UCLASS()
class PINGLEGAMEJAM_API AK_LobbyGameMode : public APingleGameJamGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Players Params")
	FPlayersRoles PlayersRoles;
	UPROPERTY(EditDefaultsOnly, Category = "PlayerStart")
	const TSubclassOf<APlayerStart> PlayerStart;
	UPROPERTY(EditDefaultsOnly, Category = "PlayerStart")
	const TSubclassOf<AK_BasePlayerController> ControllerClass;

private:
	uint32 NumberOfPlayers = 0;
	bool CycleIsDone;

	TMap<int32, bool> SpawnIsTaken;
	TArray<FTransform> SpawnSpots;	

	FTimerHandle ServerTravelTimer;
	FTimerHandle SpawnTimerHandle;

	TArray<AActor*> SpawnedControllers;

public:
	AK_LobbyGameMode();
	
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

protected:
	virtual void StartPlay() override;
	
	void StartGame();

private:
	TSubclassOf<AK_BaseCharacter> GiveRandomRole();

	void SpawnActorForNewcomer(APlayerController* NewPlayer);
	FTransform FindSpawnSpot();

	void RespawnPlayers();
	void RespawnAllDeadPlayers();
	
};

