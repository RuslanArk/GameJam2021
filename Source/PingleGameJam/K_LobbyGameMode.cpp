// Fill out your copyright notice in the Description page of Project Settings.


#include "K_LobbyGameMode.h"

#include "GJGameInstance.h"

void AK_LobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	++NumberOfPlayers;

	if (NumberOfPlayers >= 3)
	{
		GetWorldTimerManager().SetTimer(ServerTravelTimer, this, &AK_LobbyGameMode::StartGame, 2.0f);		
	}
}

void AK_LobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	--NumberOfPlayers;
}

void AK_LobbyGameMode::StartGame()
{
	UGJGameInstance* GameInstance = Cast<UGJGameInstance>(GetGameInstance());
	if (!GameInstance)	return;
	

	GameInstance->StartSession();
	
	UWorld* World = GetWorld();
	if (!World) return;

	bUseSeamlessTravel = true;

	
		
	World->ServerTravel("/Game/Maps/MapForTests?Listen");
}
