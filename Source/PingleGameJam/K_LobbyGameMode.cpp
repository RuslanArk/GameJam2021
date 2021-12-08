// Fill out your copyright notice in the Description page of Project Settings.


#include "K_LobbyGameMode.h"

#include "GJGameInstance.h"
#include "Characters/Betty/K_BettyCharacter.h"
#include "Player/K_BaseCharacter.h"

DEFINE_LOG_CATEGORY_STATIC(LogLobbyGM, All, All);

AK_LobbyGameMode::AK_LobbyGameMode()
{
	DefaultPawnClass = AK_BettyCharacter::StaticClass();
}

void AK_LobbyGameMode::StartPlay()
{
	Super::StartPlay();

}


void AK_LobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (!CycleIsDone)
	{
		for (int32 i = 0; i < PlayersRoles.PlayerClasses.Num(); i++)
		{
			PlayersRoles.RoleIsTaken.Add(i, false);
			UE_LOG(LogLobbyGM, Warning, TEXT("Index %i , role is given : %i"), i, static_cast<int32>(PlayersRoles.RoleIsTaken[i]));
		}
		CycleIsDone = true;
	}

	++NumberOfPlayers;
	
	if (GetWorld())
	{
		NewPlayer->UnPossess();
	    AK_BaseCharacter* NewCharacter = Cast<AK_BaseCharacter>(GetWorld()->SpawnActor(GiveRandomRole()));
		if (NewCharacter)
		{
			NewPlayer->Possess(NewCharacter);		
		}
	}
	
	if (NumberOfPlayers == 4)
	{
		GetWorldTimerManager().SetTimer(ServerTravelTimer, this, &AK_LobbyGameMode::StartGame, 5.0f);		
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

TSubclassOf<APawn> AK_LobbyGameMode::GiveRandomRole()
{
	// TODO: make better algorithm, this one is not efficient
	if (PlayersRoles.PlayerClasses.Num() > 0 && CycleIsDone)
	{
		const int32 RandIndex = FMath::RandRange(0, PlayersRoles.PlayerClasses.Num() - 1);

		if (!PlayersRoles.RoleIsTaken[RandIndex])
		{
			PlayersRoles.RoleIsTaken[RandIndex] = true;
			return PlayersRoles.PlayerClasses[RandIndex];
		}
	}
	
	return GiveRandomRole();
}
