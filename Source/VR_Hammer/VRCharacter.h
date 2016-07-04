// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Animation/SkeletalMeshActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "VRCharacter.generated.h"

UCLASS()
class VR_HAMMER_API AVRCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float bonus = 0;

public:
	// Sets default values for this character's properties
	AVRCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

private:
	ASkeletalMeshActor* weapon;
	USkeletalMeshComponent* weaponComponent;
	UPhysicsHandleComponent * PhysicsHandle;
	FVector initialPosition;
	bool first;
};
