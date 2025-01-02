// Fill out your copyright notice in the Description page of Project Settings.


#include "MyProject/Player/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "MyProject/Components/HealthComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//create object(camera)
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->bUsePawnControlRotation = true;

	//create object(sword mesh)
	SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sword Mesh"));
	SwordMesh->SetupAttachment(GetMesh(), FName("SwordSocket"));

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Setup for locomotion
	PlayerInputComponent->BindAxis("ForwardBackward", this, &APlayerCharacter::ForwardBackward);
	PlayerInputComponent->BindAxis("LeftRight", this, &APlayerCharacter::LeftRight);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	//Setup for cameraview
	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::Turn);
	PlayerInputComponent->BindAxis("Lookup", this, &APlayerCharacter::Lookup);

	//Setup for attack
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::StartAttack);

}

void APlayerCharacter::ForwardBackward(float InputValue)
{
	FVector ForwardDir = GetActorForwardVector(); //using vector to check the input
	AddMovementInput(ForwardDir, InputValue);
}

void APlayerCharacter::LeftRight(float InputValue)
{
	FVector LeftDir = GetActorRightVector();
	AddMovementInput(LeftDir, InputValue);
}

void APlayerCharacter::Turn(float InputValue)
{
	AddControllerYawInput(InputValue);
}

void APlayerCharacter::Lookup(float InputValue)
{
	AddControllerPitchInput(InputValue);
}

void APlayerCharacter::StartAttack()
{
	//call the animation
	if (AttackAnimation && !bIsAttacking) // avoid to attack while attacking
	{
		GetMesh()->PlayAnimation(AttackAnimation, false);// apply attack animation
		bIsAttacking = true;
	}

}

void APlayerCharacter::LineTrace()
{
	//set the area of damage from socket location
	FVector StartLocation = SwordMesh->GetSocketLocation(FName("Start")); 
	FVector EndLocation = SwordMesh->GetSocketLocation(FName("End"));
	
	//set params what happen when hit or attack enemy
	FHitResult HitResult;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);

	//linetrace
	GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, TraceParams);

	//debug line to check the attack
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 1, 0, 1);

	//function when attack/hit 
	if (HitResult.bBlockingHit)
	{
		AActor* ActorHit = HitResult.GetActor();
		UHealthComponent* EnemyHit = ActorHit->FindComponentByClass<UHealthComponent>();

		if (IsValid(EnemyHit))// do the damage when the line of attack hit the collision
		{
			EnemyHit->TakeDamage(Damage);
		}
	}
}

