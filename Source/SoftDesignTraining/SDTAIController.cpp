// Fill out your copyright notice in the Description page of Project Settings.

#include "SDTAIController.h"
#include "SoftDesignTraining.h"

void ASDTAIController::Tick(float deltaTime)
{
	APawn* pawn = GetPawn();
	ACharacter* character = Cast<ACharacter>(pawn);
	FVector direction = FVector(1, 1, 0); 
	float speed = 0.1f;
	FVector velocity = direction * speed;
	// pawn->SetActorLocation(pawn->GetActorLocation() + velocity * deltaTime); no animation and tries to clip into wall
	character->AddMovementInput(direction, 1.f); // 1.f = forward, effective range from [-1, 1]
	character->SetActorRotation(velocity.Rotation());
	character->GetCharacterMovement()->MaxWalkSpeed = 100.f;
}




