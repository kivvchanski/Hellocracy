// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ABaseActiveItem.h"

// Sets default values
AABaseActiveItem::AABaseActiveItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AABaseActiveItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AABaseActiveItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AABaseActiveItem::StartAltAction_Implementation()
{
}

void AABaseActiveItem::StopAltAction_Implementation()
{
}

void AABaseActiveItem::StartMainAction_Implementation()
{
}

void AABaseActiveItem::StopMainAction_Implementation()
{
}

void AABaseActiveItem::OnEquip_Implementation()
{

}

void AABaseActiveItem::OnUnEquip_Implementation()
{

}



