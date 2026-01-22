// Fill out your copyright notice in the Description page of Project Settings.

#include "SDTAIController.h"
#include "SoftDesignTraining.h"
#include "SDTUtils.h"
#include "Engine/OverlapResult.h"


void ASDTAIController::Tick(float deltaTime)
{	
	// Partie 1
	APawn* pawn = GetPawn();
	ACharacter* character = Cast<ACharacter>(pawn);

	// use controller desired rotation
	character->GetCharacterMovement()->bUseControllerDesiredRotation = true;

	// test turn
	float Time = GetWorld()->GetTimeSeconds();
	FVector direction = (FMath::Fmod(Time, 4.f) < 2.f)
		? FVector(1, 0, 0)
		: FVector(-1, 0, 0);

	direction.Normalize();

	// Move forward
	character->AddMovementInput(direction);

	// Smooth rotation
	FRotator currentRot = GetControlRotation();
	FRotator targetRot = direction.Rotation();

	FRotator newRot = FMath::RInterpTo(
		currentRot,
		targetRot,
		deltaTime,
		5.f
	);

	SetControlRotation(newRot);

	// Smooth speed change
	FVector forward = character->GetActorForwardVector();
	float dot = FVector::DotProduct(forward, direction);
	dot = FMath::Clamp(dot, -1.f, 1.f);

	float angle = FMath::RadiansToDegrees(FMath::Acos(dot));

	float targetSpeed = (angle > 45.f) ? 150.f : 300.f;

	float newSpeed = FMath::FInterpTo(
		character->GetCharacterMovement()->MaxWalkSpeed,
		targetSpeed,
		deltaTime,
		3.f
	);

	character->GetCharacterMovement()->MaxWalkSpeed = newSpeed;

	// Partie 2
	FVector pawnPos = pawn->GetActorLocation();
	float detectionRadius = 100.0f;
	TArray<FOverlapResult> outResults;
	SDTUtils::SphereOverlap(GetWorld(), pawnPos, 800.0f, outResults, true);

	/*FVector avoidDirection = FVector::ZeroVector;
	bool wallDetected = false;
	for (FOverlapResult& result : outResults)
	{
		AActor* otherActor = result.GetActor();
		if (otherActor && otherActor != pawn)
		{
			SDT_LOG("Detected actor: %s", *otherActor->GetName());
		}
	}	*/
	// Partie 3
	// Partie 4
	// Partie 5
	
	// Partie 6
	// Partie 7
}
