// Fill out your copyright notice in the Description page of Project Settings.

#include "Item.h"
#include "InventoryCharacter.h"
#include "Engine.h"
#include "Components/StaticMeshComponent.h" //meshes

// Sets default values
AItem::AItem()
{
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ITEM"));
	ItemMesh->SetSimulatePhysics(true);
	ItemMessage = FString("Press E to pick up.");
	ItemName = FString("Unnamed item");
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
}

void AItem::PickUpItemEvent()
{
	ItemMesh->SetSimulatePhysics(false);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ItemMesh->SetVisibility(false);
}

void AItem::PickUp()
{
	AInventoryCharacter* Character = Cast<AInventoryCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if (Character->AddItem(this))
	{
		PickUpItemEvent();
	}
}
