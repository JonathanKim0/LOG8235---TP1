// SDTAIController.h

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SDTAIController.generated.h"

UCLASS(ClassGroup = AI, config = Game)
class SOFTDESIGNTRAINING_API ASDTAIController : public AAIController
{
    GENERATED_BODY()

public:
    virtual void Tick(float deltaTime) override;

private:
    int32 lockSide = 0;
    float lockUntilTime = 0.f;

    bool bTurn90Active = false;
    float turn90TargetYaw = 0.f;

    bool lockedLeftOpen = false;
    bool lockedRightOpen = false;
    float openLockUntilTime = 0.f;

    float blockedYawSign = 0.f;

private:
    void turn90Deg(float deltaTime, float turnRateDegPerSec, float directionSign);
};
