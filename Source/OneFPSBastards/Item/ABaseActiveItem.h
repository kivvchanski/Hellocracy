// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IAlternateAction.h"
#include "IMainAction.h"
#include "ABaseActiveItem.generated.h"

UCLASS()
class ONEFPSBASTARDS_API AABaseActiveItem : public AActor, public IIAlternateAction, public IIMainAction
{
	GENERATED_BODY()
	
public:
	AABaseActiveItem();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void StartAltAction_Implementation() override;
	virtual void StopAltAction_Implementation() override;
	virtual void StartMainAction_Implementation() override;
	virtual void StopMainAction_Implementation() override;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnEquip();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnUnEquip();
	virtual void OnEquip_Implementation();
	virtual void OnUnEquip_Implementation();
};


