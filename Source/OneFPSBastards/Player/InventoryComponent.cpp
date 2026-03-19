#include "InventoryComponent.h"
#include "Item/ItemActor.h"
#include "Blueprint/UserWidget.h"

UInventoryComponent::UInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
    Super::BeginPlay();
    InventorySlots.SetNum(MaxInvetorySlots);
}

bool UInventoryComponent::AddItem(FItem Item, int32 Quantity)
{
    if (Item.ItemID.IsNone()) return false;

    // Try stacking first
    for (auto& Slot : InventorySlots) {
        if (Slot.Item.ItemID == Item.ItemID && Slot.Quantity < Item.MaxStackSize) {
            int32 SpaceLeft = Item.MaxStackSize - Slot.Quantity;
            int32 ToAdd = FMath::Min(SpaceLeft, Quantity);
            Slot.Quantity += ToAdd;
            Quantity -= ToAdd;

            if (Quantity <= 0) return true;
        }
    }

    // Add to empty slot
    for (auto& Slot : InventorySlots) {
        if (Slot.IsEmpty() || Slot.Quantity == 0) {
            Slot.Item = Item;
            Slot.Quantity = FMath::Min(Quantity, Item.MaxStackSize);
            Quantity -= Slot.Quantity;
            if (Quantity <= 0) return true;
        }
    }

    // Inventory full
    return Quantity <= 0;
}

bool UInventoryComponent::RemoveItem(FItem Item, int32 Quantity)
{
    if (Item.ItemID.IsNone()) return false;

    for (auto& Slot : InventorySlots) {
        if (Slot.Item.ItemID == Item.ItemID) {
            if (Slot.Quantity > Quantity) {
                Slot.Quantity -= Quantity;
                return true;
            }
            else {
                Slot.Item = FItem();
                Slot.Quantity = 0;
                return true;
            }
        }
    }
    return false;
}

void UInventoryComponent::DropSelectedItem()
{
    if (!GetOwner() || !InventorySlots.IsValidIndex(SelectedSlotIndex)) return;

    FInventorySlot& Slot = InventorySlots[SelectedSlotIndex];
    if (!Slot.IsEmpty() && Slot.Quantity > 0) {
        // Spawn an item pickup actor here
        FVector DropLocation = GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * 100.f;
        FRotator DropRotation = FRotator::ZeroRotator;

        // Spawn the world item actor
        AItemActor* DroppedItem = GetWorld()->SpawnActor<AItemActor>(AItemActor::StaticClass(), DropLocation, DropRotation);
        if (DroppedItem) {
            DroppedItem->ItemData = Slot.Item;
            DroppedItem->Quantity = 1; // Drop one per press, or Slot.Quantity to drop all
        }

        Slot.Quantity--;
        if (Slot.Quantity <= 0) {
            Slot.Item = FItem();
        }
    }
}

void UInventoryComponent::SelectSlot(int32 Index)
{
    if (Index >= 0 && Index < MaxInvetorySlots) {
        SelectedSlotIndex = Index;
    }
}
