// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class INVENTORY_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UPROPERTY(EditAnywhere, Category = "Item Properties")
	FString ItemName;

	UPROPERTY(EditAnywhere, Category = "Item Properties")
	class UStaticMeshComponent* ItemMesh;

	UPROPERTY(EditAnywhere, Category = "Item Properties")
	FString ItemMessage;

	UPROPERTY(EditAnywhere, Category = "Item Properties")
	UTexture2D* ItemIcon;

	void PickUpItemEvent();

	void PickUp();
	
};
