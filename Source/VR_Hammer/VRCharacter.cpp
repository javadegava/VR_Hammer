// Fill out your copyright notice in the Description page of Project Settings.

#include "VR_Hammer.h"
#include "VRCharacter.h"
#include <cmath>  


// Sets default values
AVRCharacter::AVRCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BaseEyeHeight = 0.0;

}

// Called when the game starts or when spawned
void AVRCharacter::BeginPlay()
{
	Super::BeginPlay();


	for (TActorIterator<ASkeletalMeshActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
		ASkeletalMeshActor *Mesh = *ActorItr;
		if (ActorItr->GetName().Compare("Martillo") == 0) {
			weapon = Mesh;

			TSet<UActorComponent*> componentItr = weapon->GetComponents();

			for (auto& component : componentItr)
			{
				//UE_LOG(LogTemp, Warning, TEXT("Name : %s"), *component->GetName());
				weaponComponent = (USkeletalMeshComponent*)component;
				FVector playerLocation = GetActorLocation();
				//UE_LOG(LogTemp, Warning, TEXT("X1 : %f \t Y1 : %f \t Z1 : %f"), weaponLocation.X, weaponLocation.Y, weaponLocation.Z);

			}
		}
	}
	speed = FVector();
}

// Called every frame
void AVRCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	FVector weaponLocation = weapon->GetActorLocation();
	FRotator weaponRotator = weapon->GetActorRotation();
	//UE_LOG(LogTemp, Warning, TEXT("X1 : %f \t Y1 : %f \t Z1 : %f"), weaponLocation.X, weaponLocation.Y, weaponLocation.Z);
	//UE_LOG(LogTemp, Warning, TEXT("X1 : %f \t Y1 : %f \t Z1 : %f"), weaponRotator.Roll, weaponRotator.Pitch, weaponRotator.Yaw);
	FQuat weaponQuat = weapon->GetActorQuat();

	FVector aaaa = FVector(weaponComponent->GetCenterOfMass());
	aaaa -= weaponComponent->GetBoneLocation("Root");
	float mass = weaponComponent->CalculateMass("Root");
	float distanceA = 1;
	float distanceB = aaaa.Size();
	//distanceB = 42;
	float forcePitch = mass * distanceB * this->GetWorldSettings()->GetGravityZ() * -1 / distanceA;
	//forcePitch = 4733400;




	// Impulso sujecion
	FVector force2 = FVector();
	force2.Z += mass * this->GetWorldSettings()->GetGravityZ() * -1;
	//force2.Z += forcePitch;
	weaponComponent->AddForceAtLocation(force2, weaponComponent->GetBoneLocation("Root"));






	FVector locationWeapon = FVector(weaponComponent->GetBoneLocation("Root"));
	FVector dist = FVector(0, 0, -distanceA);
	//locationWeapon.Y += distanceA;
	weaponRotator.Roll -= 90;
	weaponRotator.Yaw -= 90;
	//FRotationMatrix MyRotationMatrix(weaponRotator);
	FMatrix MyRotationMatrix = FRotationMatrix::Make(weaponQuat);
	//dist = MyRotationMatrix.TransformPosition(dist);
	dist = dist.RotateAngleAxis(weaponRotator.Roll, FVector(0, 1, 0));
	dist = dist.RotateAngleAxis(weaponRotator.Pitch, FVector(1, 0, 0));
	dist = dist.RotateAngleAxis(weaponRotator.Yaw, FVector(0, 0, 1));


	//UE_LOG(LogTemp, Warning, TEXT("X1 : %f \t Y1 : %f \t Z1 : %f"), weaponRotator.Pitch, weaponRotator.Yaw, weaponRotator.Roll);
	//UE_LOG(LogTemp, Warning, TEXT("X1 : %f \t Y1 : %f \t Z1 : %f"), locationWeapon.X, locationWeapon.Y, locationWeapon.Z);
	FVector locationPalanca = locationWeapon + dist;
	/*
	locationWeapon = locationWeapon.RotateAngleAxis(weaponRotator.Pitch, FVector(1, 0, 0));
	locationWeapon = locationWeapon.RotateAngleAxis(weaponRotator.Yaw, FVector(0, 1, 0));
	locationWeapon = locationWeapon.RotateAngleAxis(weaponRotator.Roll, FVector(0, 0, 1));*/
	//UE_LOG(LogTemp, Warning, TEXT("X2 : %f \t Y1 : %f \t Z1 : %f"), locationWeapon.X, locationWeapon.Y, locationWeapon.Z);

	
	FVector force3 = FVector();
	FVector force4 = FVector();
	//UE_LOG(LogTemp, Warning, TEXT("X1 : %f"), forcePitch);
	float angularGravity = sin((abs(weaponRotator.Pitch) + bonus) * 3.14159265 / 180);
	//UE_LOG(LogTemp, Warning, TEXT("Angulo : %f"), weaponRotator.Pitch);
	//UE_LOG(LogTemp, Warning, TEXT("Name : %f"), angularGravity);


	speed += weapon->GetVelocity();
	//UE_LOG(LogTemp, Warning, TEXT("X2 : %f \t Y1 : %f \t Z1 : %f"), speed.X, speed.Y, speed.Z);
	UE_LOG(LogTemp, Warning, TEXT("X2 : %f"), abs(weaponRotator.Pitch));

	force3.Y += forcePitch * angularGravity;
	//force3 = MyRotationMatrix.TransformVector(force3);
	force3 = force3.RotateAngleAxis(weaponRotator.Roll, FVector(0, 1, 0));
	force3 = force3.RotateAngleAxis(weaponRotator.Pitch, FVector(1, 0, 0));
	force3 = force3.RotateAngleAxis(weaponRotator.Yaw, FVector(0, 0, 1));
	force4 = force3 * -1;
	//force4 = MyRotationMatrix.TransformVector(force4);
	//UE_LOG(LogTemp, Warning, TEXT("X1 : %f \t Y1 : %f \t Z1 : %f"), weaponRotator.Roll, weaponRotator.Pitch, weaponRotator.Yaw);
	//UE_LOG(LogTemp, Warning, TEXT("XX1 : %f \t Y1 : %f \t Z1 : %f"), locationPalanca.X, locationPalanca.Y, locationPalanca.Z);
	//UE_LOG(LogTemp, Warning, TEXT("X3 : %f \t Y1 : %f \t Z1 : %f"), force3.X, force3.Y, force3.Z);
	//UE_LOG(LogTemp, Warning, TEXT("X4 : %f \t Y1 : %f \t Z1 : %f"), force4.X, force4.Y, force4.Z);
	weaponComponent->AddForceAtLocation(force3, locationPalanca);
	weaponComponent->AddForceAtLocation(force4, locationWeapon);


}

// Called to bind functionality to input
void AVRCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

