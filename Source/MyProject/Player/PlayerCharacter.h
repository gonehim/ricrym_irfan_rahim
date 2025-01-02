// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class MYPROJECT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera; //add camera for the player view

	//add sword mesh in BP_playercharacter
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* SwordMesh;

	//add attack animation
	UPROPERTY(EditAnywhere)
	class UAnimSequence* AttackAnimation;


	void ForwardBackward(float InputValue); //forward & backward
	void LeftRight(float InputValue); //left & right


	//cameraview
	void Turn(float InputValue); 
	void Lookup(float InputValue); 

	//damage declaration
	UPROPERTY(EditAnywhere)
	int Damage;

	void StartAttack();

public:

	UFUNCTION(BlueprintCallable)
	void LineTrace();

	UPROPERTY(BlueprintReadWrite)
	bool bIsAttacking;
};
