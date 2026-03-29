// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Item/Item.h"
#include "LootComponent.generated.h"

USTRUCT(BlueprintType)
struct FLootDropInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot", meta = (GetOptions = "GetItemRowNames"))
    FName ItemRowName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot", meta = (ClampMin = "0.0"))
    float DropWeight = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot", meta = (ClampMin = "0"))
    int32 MinDrops = 1;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot", meta = (ClampMin = "1"))
    int32 MaxDrops = 1;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ONEFPSBASTARDS_API ULootComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    ULootComponent();

    // Call this when the mob dies or the chest opens
    UFUNCTION(BlueprintCallable, Category = "Loot")
    TArray<FItem> GenerateLoot();

    UFUNCTION()
    TArray<FString> GetItemRowNames() const;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot")
    UDataTable* ItemDataTable;

    // The list of items this specific actor is allowed to drop
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot")
    TArray<FLootDropInfo> LootPool;
};
