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

			TArray<UActorComponent*> componentItr = weapon->GetComponents();

			for (auto& component : componentItr)
			{
				//UE_LOG(LogTemp, Warning, TEXT("Name : %s"), *component->GetName());
				weaponComponent = (USkeletalMeshComponent*)component;
				FVector playerLocation = GetActorLocation();
				//UE_LOG(LogTemp, Warning, TEXT("X1 : %f \t Y1 : %f \t Z1 : %f"), weaponLocation.X, weaponLocation.Y, weaponLocation.Z);

			}
		}
	}
}

// Called every frame
void AVRCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	FVector weaponLocation = weapon->GetActorLocation();
	FRotator weaponRotator = weapon->GetActorRotation();
	//UE_LOG(LogTemp, Warning, TEXT("X1 : %f \t Y1 : %f \t Z1 : %f"), weaponLocation.X, weaponLocation.Y, weaponLocation.Z);


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
	force2.Z += forcePitch;
	weaponComponent->AddForceAtLocation(force2, weaponComponent->GetBoneLocation("Root"));






	FVector locationWeapon = FVector(weaponComponent->GetBoneLocation("Root"));
	locationWeapon.Y += distanceA;
	//UE_LOG(LogTemp, Warning, TEXT("X1 : %f \t Y1 : %f \t Z1 : %f"), weaponRotator.Pitch, weaponRotator.Yaw, weaponRotator.Roll);
	//UE_LOG(LogTemp, Warning, TEXT("X1 : %f \t Y1 : %f \t Z1 : %f"), locationWeapon.X, locationWeapon.Y, locationWeapon.Z);
	locationWeapon = locationWeapon.RotateAngleAxis(weaponRotator.Pitch, FVector(1, 0, 0));
	locationWeapon = locationWeapon.RotateAngleAxis(weaponRotator.Yaw, FVector(0, 1, 0));
	locationWeapon = locationWeapon.RotateAngleAxis(weaponRotator.Roll, FVector(0, 0, 1));
	//UE_LOG(LogTemp, Warning, TEXT("X2 : %f \t Y1 : %f \t Z1 : %f"), locationWeapon.X, locationWeapon.Y, locationWeapon.Z);


	FVector force3 = FVector();
	force3.Z -= forcePitch;
	weaponComponent->AddForceAtLocation(force3, locationWeapon);


}

// Called to bind functionality to input
void AVRCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

