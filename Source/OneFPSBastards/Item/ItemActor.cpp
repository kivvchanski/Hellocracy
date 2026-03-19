#include "ItemActor.h"
#include "Player/MainCharacter.h"
#include "Player/InventoryComponent.h"
#include <Kismet/GameplayStatics.h>

AItemActor::AItemActor()
{
 	PrimaryActorTick.bCanEverTick = false;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);
	Mesh->SetSimulatePhysics(false);

	// Enable custom depth for outline
	Mesh->SetRenderCustomDepth(false); // default off
	Mesh->SetCustomDepthStencilValue(1);
}

void AItemActor::BeginPlay()
{
	Super::BeginPlay();

	// Ensure valid quantity
	if (Quantity <= 0) {
		Quantity = 1;
	}

	if (ItemDataTable && !ItemRowName.IsNone()) {
		//UE_LOG(LogTemp, Warning, TEXT("ItemDataTable: %s"), TEXT("YES"));
		static const FString Context = TEXT("ItemActor Context");
		FItem* Row = ItemDataTable->FindRow<FItem>(ItemRowName, Context);
		if (Row) {
			ItemData = *Row;
		}
	}
	//UE_LOG(LogTemp, Warning, TEXT("Simulating physics: %s"), Mesh->IsSimulatingPhysics() ? TEXT("YES") : TEXT("NO"));
}

void AItemActor::Interact_Implementation(AMainCharacter* InteractingCharacter)
{
	if (IsPickedUp) return; // preventes double pickup of item
	if (InteractingCharacter && InteractingCharacter->GetInventoryComponent()) {
		IsPickedUp = true;

		//UE_LOG(LogTemp, Log, TEXT("Interacted with: %s"), *GetName());

		bool IsItemAdded = InteractingCharacter->GetInventoryComponent()->AddItem(ItemData, 1);

		if (IsPickedUp && ItemData.PickupSound) {
			//UE_LOG(LogTemp, Log, TEXT("Sound played: %s"), *ItemData.DisplayName.ToString());
			UGameplayStatics::PlaySound2D(this, ItemData.PickupSound);
		}

		Destroy();
	}
}

void AItemActor::OnFocusStart_Implementation(AMainCharacter* InteractingCharacter)
{
	if (!Mesh) return;
	Mesh->SetRenderCustomDepth(true);
}
void AItemActor::OnFocusEnd_Implementation(AMainCharacter* InteractingCharacter)
{
	if (!Mesh) return;
	Mesh->SetRenderCustomDepth(false);
}
