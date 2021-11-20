
#include "K_BasePlayerController.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "K_BaseCharacter.h"


AK_BasePlayerController::AK_BasePlayerController()
{
	SetReplicates(true);
	
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AK_BasePlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

}

void AK_BasePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("UseAction", IE_Pressed, this, &AK_BasePlayerController::OnUseActionPressed);
	InputComponent->BindAction("UseAction", IE_Released, this, &AK_BasePlayerController::OnUseActionReleased);
}

void AK_BasePlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	FVector2D ScreenSpaceLocation(Location);

	// Trace to see what is under the touch location
	FHitResult HitResult;
	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		
	}
}

void AK_BasePlayerController::OnUseActionPressed()
{
	
}

void AK_BasePlayerController::OnUseActionReleased()
{
	
}
