// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"

#include "MainMenuWidget.generated.h"


UCLASS()
class PINGLEGAMEJAM_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Setup();
	void Teardown();

	void SetMenuInterface(IMenuInterface* Interface);

	protected:
	IMenuInterface* MenuInterface;
	
};
