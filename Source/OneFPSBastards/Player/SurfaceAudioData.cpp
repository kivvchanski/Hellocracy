// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/SurfaceAudioData.h"

USoundBase* USurfaceAudioData::GetSoundForSurface(EPhysicalSurface SurfaceType) const
{
    for (const auto& Mapping : SurfaceMappings) {
        if (Mapping.SurfaceType == SurfaceType) {
            if (Mapping.StepSounds.Num() > 0) {
                int32 RandomIndex = FMath::RandRange(0, Mapping.StepSounds.Num() - 1);
                return Mapping.StepSounds[RandomIndex];
            }
        }
    }
    return nullptr;
}
