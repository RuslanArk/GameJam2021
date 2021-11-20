// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "UObject/ConstructorHelpers.h"

#include "ServerRow.h"

DEFINE_LOG_CATEGORY_STATIC(LogPPMainMenu, All, All);

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/Menu/WBP_ServerRow"));
	if (!ServerRowBPClass.Class) return;

	ServerRowClass = ServerRowBPClass.Class;
}

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;
	
	if (!ensure(HostButton != nullptr)) return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::OpenHostMenu);

	if (!ensure(JoinButton != nullptr)) return false;
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if (!ensure(QuitButton != nullptr)) return false;
	QuitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitPressed);

	if (!ensure(CancelJoinButton != nullptr)) return false;
	CancelJoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	if (!ensure(JoinSessionButton != nullptr)) return false;
	JoinSessionButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

	if (!JoinAsHostButton) return false;
	JoinAsHostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
	
	if (!CancelHostButton) return false;
	CancelHostButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	return true;
}

void UMainMenu::HostServer()
{
	if (MenuInterface != nullptr)
	{
		FString ServerName = ServerNameField->Text.ToString();
		MenuInterface->Host(ServerName);
	}
}

void UMainMenu::SetServerIndex(uint32 Index)
{
	ServerIndex = Index;
	UpdateChildren();
}

void UMainMenu::UpdateChildren()
{
	for (int32 i = 0; i < ServerList->GetChildrenCount(); ++i)
	{
		UServerRow* Row = Cast<UServerRow>(ServerList->GetChildAt(i));
		if (Row != nullptr)
		{
			Row->Selected = (ServerIndex.IsSet() && ServerIndex.GetValue() == i);
		}
	}
}

void UMainMenu::JoinServer()
{
	if (ServerIndex.IsSet() && MenuInterface != nullptr)
	{
		MenuInterface->Join(ServerIndex.GetValue());
	}
	else
	{
		UE_LOG(LogPPMainMenu, Warning, TEXT("Server index is not set"));
	}
}

void UMainMenu::OpenJoinMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(JoinMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(JoinMenu);
	if (MenuInterface != nullptr)
	{
		MenuInterface->RefreshServerList();
	}
}

void UMainMenu::OpenHostMenu()
{
	MenuSwitcher->SetActiveWidget(HostMenu);
}

void UMainMenu::SetServerList(const TArray<FServerData> ServerNames)
{
	UWorld* World = GetWorld();
	if (!World) return;

	ServerList->ClearChildren();

	uint32 RowIndex = 0;
	for (const FServerData& ServerData : ServerNames)
	{
		UServerRow* Row = CreateWidget<UServerRow>(World, ServerRowClass);
		if (!Row) return;
	
		Row->ServerName->SetText(FText::FromString(ServerData.Name));
		Row->HostName->SetText(FText::FromString(ServerData.HostName));
		Row->ConnectionFraction->SetText(FText::FromString(	//
			FString::Printf(TEXT("%d / %d"), ServerData.CurrentPlayers, ServerData.MaxPlayers)));
		Row->Setup(this, RowIndex);
		++RowIndex;

		ServerList->AddChild(Row);
	}
}

void UMainMenu::OpenMainMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(JoinMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::QuitPressed()
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ConsoleCommand("quit");
}