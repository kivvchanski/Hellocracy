#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.h"
#include "Player/InteractableInterface.h"
#include "ItemActor.generated.h"

UCLASS()
class ONEFPSBASTARDS_API AItemActor : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	AItemActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UDataTable* ItemDataTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemRowName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FItem ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 Quantity = 1;

	bool IsPickedUp = false;

	virtual void Interact_Implementation(AMainCharacter* InteractingCharacter) override;
	virtual void OnFocusStart_Implementation(AMainCharacter* InteractingCharacter) override;
	virtual void OnFocusEnd_Implementation(AMainCharacter* InteractingCharacter) override;

	const FItem& GetItemData() const { return ItemData; }
	int32 GetQuantity() const { return Quantity; }

protected:
	virtual void BeginPlay() override;

};
