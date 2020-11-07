// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "InventoryGameMode.generated.h"

UCLASS(minimalapi)
class AInventoryGameMode : public AGameModeBase
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

public:
	AInventoryGameMode();

	bool GetHUD();

	UFUNCTION(BlueprintCallable, Category="HUD")
	void SetHUD(bool NewIngameHUD);

protected:
	bool IngameHUD;

	bool ChangeHUD();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD", Meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> HUDClassIngame;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD", Meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> HUDClassInventory;

	UPROPERTY()
	class UUserWidget* CurrentWidget;
};



