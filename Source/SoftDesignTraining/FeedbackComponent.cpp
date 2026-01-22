/*  #include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h" 

void UFeedbackComponent::PlayPickupFeedback(FVector Location)
{
    // Trigger Sound
    if (PickupSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, PickupSound, Location);
    }

    // Trigger FX
    if (PickupFX)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), PickupFX, Location);
    }
} */