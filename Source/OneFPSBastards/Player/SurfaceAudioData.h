// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SurfaceAudioData.generated.h"

USTRUCT(BlueprintType)
struct FSurfaceSoundMapping
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TEnumAsByte<EPhysicalSurface> SurfaceType = SurfaceType_Default;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<USoundBase*> StepSounds;
};

UCLASS()
class ONEFPSBASTARDS_API USurfaceAudioData : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category = "Footsteps")
    TArray<FSurfaceSoundMapping> SurfaceMappings;

    // Helper to find sound by surface type
    USoundBase* GetSoundForSurface(EPhysicalSurface SurfaceType) const;
};
