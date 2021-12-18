// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Menu/MenuInterface.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "GJGameInstance.generated.h"

class FOnlineSessionSearch;

UCLASS()
class PINGLEGAMEJAM_API UGJGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:
	UGJGameInstance(const FObjectInitializer& ObjectInitializer);

	TArray<FTransform> PlayerStarts;

	virtual void Init();

	UFUNCTION(BlueprintCallable)
	void LoadMenuWidget();

	UFUNCTION(BlueprintCallable)
	void InGameLoadMenu();

	UFUNCTION(Exec)
	void Host(FString ServerName) override;

	UFUNCTION(Exec)
	void Join(uint32 InIndex) override;

	UFUNCTION()
	void StartSession();

	void RefreshServerList() override;

	virtual void LoadMainMenu() override;

private:
	void OnOnlineSessionCreated(FName SessionName, bool Success);
	void OnOnlineSessionDestroyed(FName SessionName, bool Success);
	void OnFindSessionsComplete(bool Success);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnNetworkFailure(UWorld * World, UNetDriver *NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);
	
	void CreateSession();

private:
	TSubclassOf<class UUserWidget>	MenuClass;
	TSubclassOf<class UUserWidget> InGameMenuClass;

	FString DesiredServerName;

	UPROPERTY()
	class UMainMenu* Menu = nullptr;

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	
	
};
