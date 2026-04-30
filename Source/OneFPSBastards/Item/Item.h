#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Item.generated.h"

USTRUCT(BlueprintType)
struct ONEFPSBASTARDS_API FItem : public FTableRowBase
{
	GENERATED_BODY()
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FName ItemID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FText DisplayName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    bool bIsSoulBound;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    UTexture2D* Icon;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 MaxStackSize = 1;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    USoundBase* PickupSound;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    bool bConsumable;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    bool bAutoActivate;

    FItem()
        : ItemID(NAME_None)
        , DisplayName(FText::FromString("NOT DEFINED"))
        , bIsSoulBound(false)
        , Icon(nullptr)
        , MaxStackSize(1)
        , PickupSound(nullptr)
        , bConsumable(false)
        , bAutoActivate(false)
    {
    }
};
