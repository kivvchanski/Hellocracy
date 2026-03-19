// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ABaseActiveItem.h"

AABaseActiveItem::AABaseActiveItem()
{
 	PrimaryActorTick.bCanEverTick = true;
}

void AABaseActiveItem::BeginPlay()
{
	Super::BeginPlay();
}

void AABaseActiveItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AABaseActiveItem::StartAltAction_Implementation() {}
void AABaseActiveItem::StopAltAction_Implementation() {}

void AABaseActiveItem::StartMainAction_Implementation() {}
void AABaseActiveItem::StopMainAction_Implementation() {}

void AABaseActiveItem::OnEquip_Implementation() {}
void AABaseActiveItem::OnUnEquip_Implementation() {}



