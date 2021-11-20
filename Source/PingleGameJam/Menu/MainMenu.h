// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainMenuWidget.h"
#include "MainMenu.generated.h"

class UButton;
class UEditableTextBox;
class UPanelWidget;
class UUserWidget;
class UWidget;
class UWidgetSwitcher;

USTRUCT()
struct FServerData
{
	GENERATED_USTRUCT_BODY()

	FString Name;
	FString HostName;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;
};

UCLASS()
class PINGLEGAMEJAM_API UMainMenu : public UMainMenuWidget
{
	GENERATED_BODY()

public:
	UMainMenu(const FObjectInitializer& ObjectInitializer);

	void SetServerList(const TArray<FServerData> ServerNames);

	void SetServerIndex(uint32 Index);
	
	protected:
	virtual bool Initialize();

	private:

	TSubclassOf<UUserWidget> ServerRowClass;
	
	UPROPERTY(meta = (BindWidget))
	UButton* HostButton;
	
	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;

	UPROPERTY(meta = (BindWidget))
	UButton* CancelJoinButton;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinSessionButton;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinAsHostButton;

	UPROPERTY(meta = (BindWidget))
	UButton* CancelHostButton;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
	UWidget* HostMenu;

	UPROPERTY(meta = (BindWidget))
	UPanelWidget* ServerList;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* ServerNameField;

	TOptional<uint32> ServerIndex;

	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void JoinServer();

	UFUNCTION()
	void OpenMainMenu();

	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void OpenHostMenu();

	UFUNCTION()
	void QuitPressed();

	void UpdateChildren();
	
};
