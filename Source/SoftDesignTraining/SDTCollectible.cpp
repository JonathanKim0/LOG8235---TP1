// Fill out your copyright notice in the Description page of Project Settings.

#include "SDTCollectible.h"
#include "SoftDesignTraining.h"
#include "Kismet/GameplayStatics.h"
//#include "NiagaraFunctionLibrary.h"

ASDTCollectible::ASDTCollectible()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASDTCollectible::BeginPlay()
{
    Super::BeginPlay();
}

void ASDTCollectible::Collect()
{
    GetWorld()->GetTimerManager().SetTimer(m_CollectCooldownTimer, this, &ASDTCollectible::OnCooldownDone, m_CollectCooldownDuration, false);
    PlayPickupFeedback();
    GetStaticMeshComponent()->SetVisibility(false);
}

void ASDTCollectible::OnCooldownDone()
{
    GetWorld()->GetTimerManager().ClearTimer(m_CollectCooldownTimer);

    GetStaticMeshComponent()->SetVisibility(true);
}   

bool ASDTCollectible::IsOnCooldown()
{
    return m_CollectCooldownTimer.IsValid();
}

void ASDTCollectible::Tick(float deltaTime)
{
    Super::Tick(deltaTime);
}

void ASDTCollectible::PlayPickupFeedback()
{
    // Trigger Sound
    if (PickupSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
    }

    // Trigger FX
    /*if (PickupFX)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), PickupFX, GetActorLocation());
    } */ 
}
