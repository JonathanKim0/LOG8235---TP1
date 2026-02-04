// SDTAIController.cpp

#include "SDTAIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "SDTUtils.h"
#include "Engine/OverlapResult.h"

void ASDTAIController::turn90Deg(float deltaTime, float turnRateDegPerSec, float directionSign)
{
    ACharacter* character = Cast<ACharacter>(GetPawn());
    if (!character) return;

    const float curYaw = character->GetActorRotation().Yaw;

    if (!bTurn90Active)
    {
        bTurn90Active = true;
        turn90TargetYaw = FRotator::NormalizeAxis(curYaw + directionSign * 90.f);
    }

    // get closest angle to target yaw
    const float deltaYaw = FMath::FindDeltaAngleDegrees(curYaw, turn90TargetYaw);
    const float stopEpsilonDeg = 2.f;

    // dont turn when close to Yaw to avoid jitters
    if (FMath::Abs(deltaYaw) <= stopEpsilonDeg)
    {
        bTurn90Active = false;
        return;
    }

    const float signedRate = (deltaYaw > 0.f) ? turnRateDegPerSec : -turnRateDegPerSec;

    FRotator rot = character->GetActorRotation();
    rot.Yaw = FRotator::NormalizeAxis(rot.Yaw + signedRate * deltaTime);
    character->SetActorRotation(rot);
}

void ASDTAIController::Tick(float deltaTime)
{
    Super::Tick(deltaTime);

    ACharacter* character = Cast<ACharacter>(GetPawn());
    if (!character) return;

    UCharacterMovementComponent* move = character->GetCharacterMovement();
    if (!move) return;

    // control yaw
    move->bUseControllerDesiredRotation = false;
    move->bOrientRotationToMovement = false;
    character->bUseControllerRotationYaw = false;

    UWorld* world = GetWorld();
    if (!world) return;

    const float now = world->GetTimeSeconds();

    const float moveSpeed = 300.f;
    const float traceLen = 150.f;
    const float sideTraceLen = traceLen * 0.95f;
    const float turnRateDegPerSec = 270.f;
    const float sideDeg = 35.f;
    const float lockDuration = 1.f;
    const float sideOpenDist = 300.f;
    const float turningMoveScale = 0.7f;
    const ECollisionChannel deathFloor = ECC_GameTraceChannel3;
    const ECollisionChannel player = ECC_GameTraceChannel4;
    const ECollisionChannel collectible = ECC_GameTraceChannel5;

    move->MaxWalkSpeed = moveSpeed;

    const FVector pos = character->GetActorLocation();
    const FVector fwd = character->GetActorForwardVector();
    const FVector right = character->GetActorRightVector();
    const FVector up = FVector::UpVector;

    const FVector fwdLeft = fwd.RotateAngleAxis(-sideDeg, up).GetSafeNormal();
    const FVector fwdRight = fwd.RotateAngleAxis(+sideDeg, up).GetSafeNormal();

    // forward ray
    const bool wallAhead = SDTUtils::Raycast(world, pos, pos + fwd * traceLen);
    DrawDebugLine(world, pos, pos + fwd * traceLen, wallAhead ? FColor::Red : FColor::Green, false, 0.f, 0, 2.f);

    // angled rays
    float leftDist = sideTraceLen;
    const bool leftHit = SDTUtils::RaycastDistance(world, pos, pos + fwdLeft * sideTraceLen, character, leftDist);
    DrawDebugLine(world, pos, pos + fwdLeft * sideTraceLen, leftHit ? FColor::Red : FColor::Green, false, 0.f, 0, 2.f);
    float rightDist = sideTraceLen;
    const bool rightHit = SDTUtils::RaycastDistance(world, pos, pos + fwdRight * sideTraceLen, character, rightDist);
    DrawDebugLine(world, pos, pos + fwdRight * sideTraceLen, rightHit ? FColor::Red : FColor::Green, false, 0.f, 0, 2.f);

    // side rays
    float leftSideDist = sideOpenDist;
    const bool leftSideHit = SDTUtils::RaycastDistance(world, pos, pos - right * sideOpenDist, character, leftSideDist);
    DrawDebugLine(world, pos, pos - right * sideOpenDist, leftSideHit ? FColor::Red : FColor::Green, false, 0.f, 0, 2.f);
    float rightSideDist = sideOpenDist;
    const bool rightSideHit = SDTUtils::RaycastDistance(world, pos, pos + right * sideOpenDist, character, rightSideDist);
    DrawDebugLine(world, pos, pos + right * sideOpenDist, rightSideHit ? FColor::Red : FColor::Green, false, 0.f, 0, 2.f);

    bool leftLocked = leftHit;
    bool rightLocked = rightHit;

    // Collision detection
    TArray<FOverlapResult> collectibleResults;
    TArray<ECollisionChannel> collectibleFilter;
    collectibleFilter.Add(collectible);
    bool seeCollectible = SDTUtils::BoxOverlap(world, pos, character->GetActorQuat(), 800, 200, collectibleResults, collectibleFilter, true);

    TArray<FOverlapResult> deathResults;
    TArray<ECollisionChannel> deathFilter;
    deathFilter.Add(deathFloor);
    bool seeDeathFloor = SDTUtils::BoxOverlap(world, pos, character->GetActorQuat(), 150, 150, deathResults, deathFilter, true);

    TArray<FOverlapResult> playerResults;
    TArray<ECollisionChannel> playerFilter;
    playerFilter.Add(player);
    bool hearPlayer = SDTUtils::SphereOverlap(world, pos, 800, playerResults, playerFilter, true);
    
    for (FOverlapResult result : collectibleResults) {
    }

    for (FOverlapResult result : deathResults) {
    }

    for (FOverlapResult result : playerResults) {
    }

    // lock direction temporarily on angled hit
    if (leftHit && !rightHit)
    {
        lockSide = -1;
        lockUntilTime = now + lockDuration;
    }
    else if (rightHit && !leftHit)
    {
        lockSide = +1;
        lockUntilTime = now + lockDuration;
    }

    if (now < lockUntilTime)
    {
        if (lockSide == -1) rightLocked = false;
        if (lockSide == +1) leftLocked = false;
    }
    else
    {
        lockSide = 0;
    }

    float fineYawRate = 0.f;

    if (wallAhead)
    {
        if (now >= openLockUntilTime)
        {
            lockedLeftOpen = (leftSideDist >= sideOpenDist);
            lockedRightOpen = (rightSideDist >= sideOpenDist);
            openLockUntilTime = now + lockDuration;

            // turn 90deg into side with an opening
            if (lockedLeftOpen && !lockedRightOpen) blockedYawSign = -1.f;
            else if (lockedRightOpen && !lockedLeftOpen) blockedYawSign = +1.f;
            else blockedYawSign = (FMath::FRand() < 0.5f) ? +1.f : -1.f;

            bTurn90Active = false;
        }

        turn90Deg(deltaTime, turnRateDegPerSec, blockedYawSign);
    }
    else
    {
        // release lock when fwd ray is free
        openLockUntilTime = 0.f;
        bTurn90Active = false;

        if (rightLocked && !leftLocked) fineYawRate -= turnRateDegPerSec;
        if (leftLocked && !rightLocked) fineYawRate += turnRateDegPerSec;

        if (FMath::Abs(fineYawRate) > KINDA_SMALL_NUMBER)
        {
            FRotator rot = character->GetActorRotation();
            rot.Yaw = FRotator::NormalizeAxis(rot.Yaw + fineYawRate * deltaTime);
            character->SetActorRotation(rot);
        }
    }

    const bool isTurning = wallAhead || (FMath::Abs(fineYawRate) > KINDA_SMALL_NUMBER);
    const float moveScale = isTurning ? turningMoveScale : 1.f; // slowdown when turnings

    character->AddMovementInput(fwd, moveScale);
}
