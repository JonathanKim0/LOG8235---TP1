// Fill out your copyright notice in the Description page of Project Settings.

#include "SDTUtils.h"
#include "SoftDesignTraining.h"
#include "SoftDesignTrainingMainCharacter.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "CollisionQueryParams.h"
#include "Engine/OverlapResult.h"

/*static*/ bool SDTUtils::Raycast(UWorld* uWorld, FVector sourcePoint, FVector targetPoint)
{
    FHitResult hitData;
    FCollisionQueryParams TraceParams(FName(TEXT("VictoreCore Trace")), true);

    return uWorld->LineTraceSingleByChannel(hitData, sourcePoint, targetPoint, ECC_Pawn, TraceParams);
}

/*static*/ bool SDTUtils::RaycastDistance(UWorld* uWorld, FVector sourcePoint, FVector targetPoint, const AActor* ignoreActor, float& outDistance)
{
    FHitResult hitData;

    FCollisionQueryParams traceParams(FName(TEXT("SDT RaycastDistance")), true);
    if (ignoreActor) traceParams.AddIgnoredActor(ignoreActor);

    const bool bHit = uWorld->LineTraceSingleByChannel(hitData, sourcePoint, targetPoint, ECC_Pawn, traceParams);

    const float len = (targetPoint - sourcePoint).Size();
    outDistance = (bHit && hitData.bBlockingHit) ? hitData.Distance : len;

    return bHit && hitData.bBlockingHit;
}

bool SDTUtils::IsPlayerPoweredUp(UWorld * uWorld)
{
    ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(uWorld, 0);
    if (!playerCharacter)
        return false;

    ASoftDesignTrainingMainCharacter* castedPlayerCharacter = Cast<ASoftDesignTrainingMainCharacter>(playerCharacter);
    if (!castedPlayerCharacter)
        return false;

    return castedPlayerCharacter->IsPoweredUp();
}

bool SDTUtils::SphereOverlap(
    UWorld* uWorld,
    const FVector& pos,
    float radius,
    TArray<struct FOverlapResult>& outOverlaps,
    TArray<ECollisionChannel> filter,
    bool drawDebug
)
{
    if (uWorld == nullptr)
        return false;

    if (drawDebug)
        DrawDebugSphere(uWorld, pos, radius, 24, FColor::Green);

    FCollisionObjectQueryParams params;

    for (ECollisionChannel f : filter) {
        params.AddObjectTypesToQuery(f);
    }

    FCollisionShape collisionShape;
    collisionShape.SetSphere(radius);
    FCollisionQueryParams queryParams = FCollisionQueryParams::DefaultQueryParam;
    queryParams.bReturnPhysicalMaterial = true;

    uWorld->OverlapMultiByObjectType(outOverlaps, pos, FQuat::Identity, params, collisionShape, queryParams);

    //Draw overlap results
    if (drawDebug)
    {
        for (int32 i = 0; i < outOverlaps.Num(); ++i)
        {
            if (outOverlaps[i].GetComponent())
                DebugDrawPrimitive(uWorld, *(outOverlaps[i].GetComponent()));
        }
    }

    return outOverlaps.Num() > 0;
}

bool SDTUtils::BoxOverlap(
    UWorld* uWorld,
    const FVector& pos,
    FQuat rotation,
    float length,
    float width,
    TArray<struct FOverlapResult>& outOverlaps,
    TArray<ECollisionChannel> filter,
    bool drawDebug
)
{
    if (uWorld == nullptr)
        return false;

    if (drawDebug)
        DrawDebugBox(
            uWorld,
            pos + rotation.RotateVector(FVector(length / 2, 0, 0)),
            FVector(length / 2, width / 2, 100),
            rotation,
            FColor::Blue
        );

    FCollisionObjectQueryParams params;

    for (ECollisionChannel f : filter) {
        params.AddObjectTypesToQuery(f);
    }

    FCollisionShape collisionShape;
    collisionShape.SetBox(FVector3f(length / 2, width / 2, 100));
    FCollisionQueryParams queryParams = FCollisionQueryParams::DefaultQueryParam;
    queryParams.bReturnPhysicalMaterial = true;

    uWorld->OverlapMultiByObjectType(outOverlaps, pos + rotation.RotateVector(FVector(length / 2, 0, 0)), rotation, params, collisionShape, queryParams);

    //Draw overlap results
    if (drawDebug)
    {
        for (int32 i = 0; i < outOverlaps.Num(); ++i)
        {
            if (outOverlaps[i].GetComponent())
                DebugDrawPrimitive(uWorld, *(outOverlaps[i].GetComponent()));
        }
    }

    return outOverlaps.Num() > 0;
}

void SDTUtils::DebugDrawPrimitive(UWorld* uWorld, const UPrimitiveComponent& primitive)
{
    FVector center = primitive.Bounds.Origin;
    FVector extent = primitive.Bounds.BoxExtent;

    DrawDebugBox(uWorld, center, extent, FColor::Red);
}
