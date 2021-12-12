// Fill out your copyright notice in the Description page of Project Settings.


#include "K_LobbyGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"

#include "GJGameInstance.h"
#include "Player/K_BaseCharacter.h"

DEFINE_LOG_CATEGORY_STATIC(LogLobbyGM, All, All);

AK_LobbyGameMode::AK_LobbyGameMode()
{
	CycleIsDone = false;
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
		}
		
		TArray<AActor*> FoundStarts;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), PlayerStart, FoundStarts);		
		if (FoundStarts.Num() > 0)
		{
			int32 Index = 0;
			for (AActor* Start : FoundStarts)
			{
				SpawnSpots.Add(Start->GetActorTransform());
				SpawnIsTaken.Add(Index, false);
				Index++;
			}
		}
		
		CycleIsDone = true;
	}

	++NumberOfPlayers;
	
	SpawnActorForNewcomer(NewPlayer);
	
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

	/*bUseSeamlessTravel = true;
	World->ServerTravel("/Game/Maps/Build1?Listen", true);
	CycleIsDone = false;*/
}

void AK_LobbyGameMode::SpawnActorForNewcomer(APlayerController* NewPlayer)
{
	if (UWorld* World = GetWorld())
	{
		FTransform SpawnSpot = FindSpawnSpot();
		AK_BaseCharacter* NewCharacter = GetWorld()->SpawnActorDeferred<AK_BaseCharacter>(GiveRandomRole(), SpawnSpot);
		if (NewCharacter)
		{
			NewPlayer->GetPawn()->SetLifeSpan(1.0f);
			NewPlayer->UnPossess();
			NewPlayer->Possess(NewCharacter);
			NewCharacter->FinishSpawning(SpawnSpot);
		}
	}
}

TSubclassOf<AK_BaseCharacter> AK_LobbyGameMode::GiveRandomRole()
{
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

FTransform AK_LobbyGameMode::FindSpawnSpot()
{
	if (SpawnSpots.Num() > 0 && CycleIsDone)
	{
		const int32 RandIndex = FMath::RandRange(0, SpawnSpots.Num() - 1);
		if (!SpawnIsTaken[RandIndex])
		{
			SpawnIsTaken[RandIndex] = true;
			return SpawnSpots[RandIndex];
		}
	}
	return FindSpawnSpot();
}
