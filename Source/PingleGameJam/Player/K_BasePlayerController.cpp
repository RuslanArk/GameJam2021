
#include "K_BasePlayerController.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "K_BaseCharacter.h"


AK_BasePlayerController::AK_BasePlayerController()
{
	SetReplicates(true);
	
	//bShowMouseCursor = true;
	//DefaultMouseCursor = EMouseCursor::Crosshairs;
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
}

void AK_BasePlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

}

void AK_BasePlayerController::OnUseActionPressed()
{
	// Trace to see what is under the mouse cursor
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Pawn, false, Hit);

	if (Hit.bBlockingHit)
	{
		// We hit something, move there
		//SetAttack(Hit.ImpactPoint);
	}
}

void AK_BasePlayerController::OnUseActionReleased()
{
	
}

void AK_BasePlayerController::Server_UpdateCursorLocation_Implementation(FVector Location)
{
	Server_MouseCursor = Location;
}
