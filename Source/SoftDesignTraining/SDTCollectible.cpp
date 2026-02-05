// Fill out your copyright notice in the Description page of Project Settings.

#include "SDTCollectible.h"
#include "SoftDesignTraining.h"
#include "Kismet/GameplayStatics.h"

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
    if (IsOnCooldown()) return;
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
    if (PickupSound)
    {
        UGameplayStatics::SpawnSoundAtLocation(GetWorld(), PickupSound, GetActorLocation());
    }

    if (PickupFX)
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PickupFX, GetActorLocation(),GetActorRotation(), true);
    } 
}
