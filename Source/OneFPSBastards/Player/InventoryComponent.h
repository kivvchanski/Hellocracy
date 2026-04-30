#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item/Item.h"
#include "InventoryComponent.generated.h"

USTRUCT(BlueprintType)
struct FInventorySlot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    FItem Item;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    int32 Quantity = 0;

    bool IsEmpty() const
    {
        return Item.ItemID.IsNone() || Quantity <= 0;
    }
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ONEFPSBASTARDS_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    int32 MaxInvetorySlots = 8;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
    TArray<FInventorySlot> InventorySlots;

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool AddItem(FItem Item, int32 Quantity = 1);
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool RemoveItem(FItem Item, int32 Quantity = 1);
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void DropSelectedItem();
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void SelectSlot(int32 Index);
    UFUNCTION(BlueprintPure, Category = "Inventory")
    int32 GetSelectedSlot() const { return SelectedSlotIndex; }
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    int32 FindCountOfItems(FName ItemName);

private:
    int32 SelectedSlotIndex = 0;
};
