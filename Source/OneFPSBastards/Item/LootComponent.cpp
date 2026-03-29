// Fill out your copyright notice in the Description page of Project Settings.


#include "LootComponent.h"
#include "Math/UnrealMathUtility.h"

ULootComponent::ULootComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

TArray<FItem> ULootComponent::GenerateLoot()
{
    TArray<FItem> GeneratedLoot;

    if (!ItemDataTable || LootPool.IsEmpty()) {
        UE_LOG(LogTemp, Warning, TEXT("LootComponent is missing DataTable or LootPool is empty!"));
        return GeneratedLoot;
    }

    float TotalWeight = 0.0f;
    for (const FLootDropInfo& DropInfo : LootPool) {
        TotalWeight += DropInfo.DropWeight;
    }
    if (TotalWeight <= 0.0f) return GeneratedLoot;

    for (const FLootDropInfo& DropInfo : LootPool) {
        float DropChance = DropInfo.DropWeight / TotalWeight;

        if (FMath::FRand() <= DropChance) {
            FItem* FoundItem = ItemDataTable->FindRow<FItem>(DropInfo.ItemRowName, TEXT("LootGeneration"));

            if (FoundItem) {
                int32 ItemQuantity = FMath::RandRange(DropInfo.MinDrops, DropInfo.MaxDrops);

                for (int32 q = 0; q < ItemQuantity; ++q) {
                    GeneratedLoot.Add(*FoundItem);
                }
            }
        }
    }

    return GeneratedLoot;
}

TArray<FString> ULootComponent::GetItemRowNames() const
{
    TArray<FString> RowNames;

    RowNames.Add(TEXT("None"));

    if (ItemDataTable) {
        for (const FName& Name : ItemDataTable->GetRowNames()) {
            RowNames.Add(Name.ToString());
        }
    }

    return RowNames;
}