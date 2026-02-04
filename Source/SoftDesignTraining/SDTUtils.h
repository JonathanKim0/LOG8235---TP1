// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define COLLISION_DEATH_OBJECT		ECollisionChannel::ECC_GameTraceChannel3
#define COLLISION_PLAYER        	ECollisionChannel::ECC_GameTraceChannel4
#define COLLISION_COLLECTIBLE     	ECollisionChannel::ECC_GameTraceChannel5

class SOFTDESIGNTRAINING_API SDTUtils
{
public:
    static bool Raycast(UWorld* uWorld, FVector sourcePoint, FVector targetPoint);
    static bool RaycastDistance(UWorld* uWorld, FVector sourcePoint, FVector targetPoint, const AActor* ignoreActor, float& outDistance);
    static bool IsPlayerPoweredUp(UWorld* uWorld);
    static bool SphereOverlap(UWorld* uWorld, const FVector& pos, float radius, TArray<struct FOverlapResult>& outOverlaps, TArray<ECollisionChannel> filter, bool drawDebug);
    static bool BoxOverlap(UWorld* uWorld, const FVector& pos, FQuat rotation, float length, float width, TArray<struct FOverlapResult>& outOverlaps, TArray<ECollisionChannel> filter, bool drawDebug);
    static void DebugDrawPrimitive(UWorld* uWorld, const UPrimitiveComponent& primitive);
};
