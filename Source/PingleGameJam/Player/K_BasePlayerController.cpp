
#include "K_BasePlayerController.h"

#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

#include "K_BaseCharacter.h"
#include "K_BasePlayerState.h"
#include "GameFramework/GameModeBase.h"
#include "PingleGameJam/K_LobbyGameMode.h"

DEFINE_LOG_CATEGORY_STATIC(LogK_PlayerController, All, All);

AK_BasePlayerController::AK_BasePlayerController()
{
	SetReplicates(true);
}

void AK_BasePlayerController::TriggerRespawn()
{	
	//GetWorldTimerManager().SetTimer(RespawnTimer, this, &AK_BasePlayerController::Server_RespawnPlayer_Implementation, 2.0f);
}

void AK_BasePlayerController::Server_RespawnPlayer_Implementation()
{
	if (HasAuthority())
	{		
		AK_BasePlayerState* CurrentPlayerState = GetPlayerState<AK_BasePlayerState>();
		if (!CurrentPlayerState || !CurrentPlayerState->GetCharacter()) return;	
		TSubclassOf<AK_BaseCharacter> PlayerClass = CurrentPlayerState->GetCharacter()->GetClass();
		if (!PlayerClass) return;
	
		FTransform SpawnSpot = FindSpawnPlace();
		AK_BaseCharacter* CharacterToSpawn = GetWorld()->SpawnActorDeferred<AK_BaseCharacter>(PlayerClass, SpawnSpot);
		if (CharacterToSpawn)
		{
			if (GetPawn())
			{
				GetPawn()->SetLifeSpan(1.0f);
				UnPossess();
			}
			Possess(CharacterToSpawn);
			CharacterToSpawn->FinishSpawning(SpawnSpot);
		}
	}
}

void AK_BasePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("UseAction", IE_Pressed, this, &AK_BasePlayerController::OnUseActionPressed);
	InputComponent->BindAction("UseAction", IE_Released, this, &AK_BasePlayerController::OnUseActionReleased);
}

void AK_BasePlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	
	SetShowMouseCursor(true);

	AK_BaseCharacter* OwnedPawn = Cast<AK_BaseCharacter>(GetPawn());
	if (OwnedPawn)
	{
		OwnedPawn->EventOnCharacterDied.AddUObject(this, &AK_BasePlayerController::Server_RespawnPlayer_Implementation);
	}
}

void AK_BasePlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	
	// Trace to see what is under the mouse cursor
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Pawn, false, Hit);
	if (Hit.bBlockingHit)
	{
		Server_UpdateCursorLocation(Hit.ImpactPoint);
	}
	
}

void AK_BasePlayerController::OnUseActionPressed()
{
	// Trace to see what is under the mouse cursor
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Pawn, false, Hit);

	if (Hit.bBlockingHit)
	{
		if (auto MyCharacter = Cast<AK_BaseCharacter>(GetPawn()))
		{
			MyCharacter->Server_ActivateAbility(0, Hit.ImpactPoint, nullptr);
		}
	}
}

void AK_BasePlayerController::OnUseActionReleased()
{
	
}

FTransform AK_BasePlayerController::FindSpawnPlace()
{
	if (!GetWorld() || !PlayerStartClass) return FTransform(FVector::ZeroVector);
	
	TArray<AActor*> FoundStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), PlayerStartClass, FoundStarts);		
	if (FoundStarts.Num() > 0)
	{
		int32 RandSpotIndex = FMath::RandRange(0, FoundStarts.Num() -1);
		return FoundStarts[RandSpotIndex]->GetTransform();
	}
	return FTransform(FVector::ZeroVector);
}

void AK_BasePlayerController::Server_UpdateCursorLocation_Implementation(FVector Location)
{
	Server_MouseCursor = Location;
}
