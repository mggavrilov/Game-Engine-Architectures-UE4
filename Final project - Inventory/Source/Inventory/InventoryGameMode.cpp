// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "InventoryGameMode.h"
#include "InventoryHUD.h"
#include "InventoryCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void AInventoryGameMode::BeginPlay()
{
	ChangeHUD();
}

AInventoryGameMode::AInventoryGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AInventoryHUD::StaticClass();

	IngameHUD = true;
}

bool AInventoryGameMode::GetHUD()
{
	return IngameHUD;
}

void AInventoryGameMode::SetHUD(bool NewIngameHUD)
{
	IngameHUD = NewIngameHUD;

	ChangeHUD();
}

bool AInventoryGameMode::ChangeHUD()
{
	//https://wiki.unrealengine.com/Trace_Functions
	AInventoryCharacter* Character = Cast<AInventoryCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();

	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
	}

	if (IngameHUD)
	{
		Controller->bShowMouseCursor = false;
		Controller->bEnableClickEvents = false;

		//allow player and camera movement after exiting the inventory screen
		//Controller->ClientIgnoreMoveInput(false);
		Controller->ClientIgnoreLookInput(false);
		FInputModeGameOnly Mode;
		Controller->SetInputMode(Mode);

		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDClassIngame);
	}
	else
	{
		Controller->bShowMouseCursor = true;
		Controller->bEnableClickEvents = true;

		//disallow player and camera movement while on the inventory screen
		//Controller->ClientIgnoreMoveInput(true);
		Controller->ClientIgnoreLookInput(true);

		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDClassInventory);

		//https://answers.unrealengine.com/questions/130528/c-create-widget.html?sort=oldest
		//disable the game input and focus on the inventory
		FInputModeGameAndUI Mode;
		Mode.SetWidgetToFocus(CurrentWidget->TakeWidget());
		Controller->SetInputMode(Mode);
	}
		
	if (CurrentWidget)
	{
		CurrentWidget->AddToViewport();
		return true;
	}
	else
	{
		return false;
	}
}
