// Fill out your copyright notice in the Description page of Project Settings.


#include "GJGameInstance.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"

#include "Menu/MainMenu.h"
#include "Menu/MainMenuWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogPPGameInstance, All, All);

//const static FName SESSION_NAME = TEXT("My session name");
const static FName SERVER_NAME_KEY = TEXT("SessionName");

UGJGameInstance::UGJGameInstance(const FObjectInitializer & ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/Menu/WBP_MainMenu"));
	if (!ensure(MenuBPClass.Class != nullptr)) return;

	MenuClass = MenuBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/Menu/WBP_InGameMenu"));
	if (!ensure(InGameMenuBPClass.Class != nullptr)) return;

	InGameMenuClass = InGameMenuBPClass.Class;
}

void UGJGameInstance::Init()
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		UE_LOG(LogPPGameInstance, Warning, TEXT("Found Online Subsystem: %s"), *OnlineSubsystem->GetSubsystemName().ToString());
		SessionInterface = OnlineSubsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{			
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UGJGameInstance::OnOnlineSessionCreated);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UGJGameInstance::OnOnlineSessionDestroyed);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UGJGameInstance::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UGJGameInstance::OnJoinSessionComplete);
		}		
	}
	else
	{
		UE_LOG(LogPPGameInstance, Warning, TEXT("Found no Online Subsystem"));
	}
	UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *MenuClass->GetName());

	if (GEngine != nullptr)
	{
		GEngine->OnNetworkFailure().AddUObject(this, &UGJGameInstance::OnNetworkFailure);
	}
}

void UGJGameInstance::LoadMenuWidget()
{
	if (!ensure(MenuClass != nullptr)) return;

	Menu = CreateWidget<UMainMenu>(this, MenuClass);
	if (!ensure(Menu != nullptr)) return;

	Menu->Setup();

	Menu->SetMenuInterface(this);
}

void UGJGameInstance::InGameLoadMenu()
{
	if (!ensure(InGameMenuClass != nullptr)) return;

	UMainMenuWidget* MainMenu = CreateWidget<UMainMenuWidget>(this, InGameMenuClass);
	if (!ensure(MainMenu != nullptr)) return;

	MainMenu->Setup();

	MainMenu->SetMenuInterface(this);
}

void UGJGameInstance::Host(FString ServerName)
{
	DesiredServerName = ServerName;
	if (SessionInterface.IsValid())
	{
		auto ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession);
		if (ExistingSession != nullptr)
		{
			SessionInterface->DestroySession(NAME_GameSession);
		}
		else
		{
			CreateSession();
		}		
	}
}

void UGJGameInstance::Join(uint32 InIndex)
{
	if (!SessionSearch.IsValid()) return;
	if (!SessionInterface.IsValid()) return;
	
	if (Menu != nullptr)
	{
		Menu->Teardown();
	}

	SessionInterface->JoinSession(0, NAME_GameSession, SessionSearch->SearchResults[InIndex]);
	
}

void UGJGameInstance::StartSession()
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->StartSession(NAME_Game);
	}
}

void UGJGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!SessionInterface.IsValid()) return;
	
	FString Address;
	if (!SessionInterface->GetResolvedConnectString(SessionName, Address))
	{
		UE_LOG(LogPPGameInstance, Warning, TEXT("Couldn't join the session, there is no Address"));
		return;
	}
	
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;
	
	Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));
	
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;
	
	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UGJGameInstance::LoadMainMenu()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!PlayerController) return;

	PlayerController->ClientTravel("/Game/Menu/MainMenu", ETravelType::TRAVEL_Absolute);
}

void UGJGameInstance::OnOnlineSessionCreated(FName SessionName, bool Success)
{
	if (!Success)
	{
		UE_LOG(LogPPGameInstance, Warning, TEXT("No session created"));
		return;
	}
	
	if (Menu != nullptr)
	{
		Menu->Teardown();
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	World->ServerTravel("/Game/Maps/Lobby?listen");
}

void UGJGameInstance::OnOnlineSessionDestroyed(FName SessionName, bool Success)
{
	if (Success)
	{
		CreateSession();
	}
}

void UGJGameInstance::RefreshServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch);
	if (SessionSearch.IsValid())
	{
		UE_LOG(LogPPGameInstance, Warning, TEXT("Starting finding session"));
		SessionSearch->bIsLanQuery = false;
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());				
	}
}

void UGJGameInstance::OnFindSessionsComplete(bool Success)
{
	UE_LOG(LogPPGameInstance, Warning, TEXT("Finishing finding session"));
	if (Success && SessionSearch.IsValid() && Menu != nullptr)
	{
		TArray<FServerData> ServerNames;
		for (FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
		{
			UE_LOG(LogPPGameInstance, Warning, TEXT("Found session: %s"), *SearchResult.GetSessionIdStr());
			
			FServerData NewData;
			NewData.HostName = SearchResult.Session.OwningUserName;
			NewData.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
			NewData.CurrentPlayers = NewData.MaxPlayers - SearchResult.Session.NumOpenPublicConnections;

			FString ServerName;
			if (SearchResult.Session.SessionSettings.Get(SERVER_NAME_KEY, ServerName))
			{				
				NewData.Name = ServerName;
			}
			else
			{
				NewData.Name = TEXT("Couldn't receive Server Name");
			}
			
			
			ServerNames.Add(NewData);
		}
		Menu->SetServerList(ServerNames);
	}
}

void UGJGameInstance::OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString = TEXT("Error in network Driver"))
{
	LoadMainMenu();
}

void UGJGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;
		if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
		{
			SessionSettings.bIsLANMatch = true;
		}
		else
		{
			SessionSettings.bIsLANMatch = false;
		}	
		SessionSettings.NumPublicConnections = 5;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.Set(SERVER_NAME_KEY, DesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		
		SessionInterface->CreateSession(0, NAME_GameSession, SessionSettings);
	}
}
