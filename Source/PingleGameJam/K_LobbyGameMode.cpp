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

void AK_LobbyGameMode::SpawnActorForNewcomer(APlayerController* NewPlayer)
{
	if (UWorld* World = GetWorld())
	{		
		TArray<AActor*> FoundStarts;
		UGameplayStatics::GetAllActorsOfClass(World, PlayerStart, FoundStarts);

		TArray<FTransform> SpawnPoints;
		if (FoundStarts.Num() > 0)
		{
			for (AActor* Start : FoundStarts)
			{
				SpawnPoints.Add(Start->GetActorTransform());
			}
		}

		FTransform SpawnTransform = SpawnPoints.Num() > 0 ? SpawnPoints[0] : FTransform(FVector::ZeroVector);
		AK_BaseCharacter* NewCharacter = GetWorld()->SpawnActorDeferred<AK_BaseCharacter>(GiveRandomRole(), SpawnTransform);
		if (NewCharacter)
		{
			NewPlayer->GetPawn()->SetLifeSpan(1.0f);
			NewPlayer->UnPossess();
			NewPlayer->Possess(NewCharacter);
			NewCharacter->FinishSpawning(SpawnTransform);
			UE_LOG(LogLobbyGM, Warning, TEXT("Controller Possessed"));
		}
	}
}
