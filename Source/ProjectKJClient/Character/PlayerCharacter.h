// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GroomComponent.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class PROJECTKJCLIENT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();
private:
	//UPROPERTY(EditAnywhere, Category = "SkeletalMesh", meta = (AllowPrivateAccess = "true"))
	//USkeletalMeshComponent* SkeletalBody;
	UPROPERTY(EditAnywhere, Category = "SkeletalMesh", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SkeletalFeet;
	UPROPERTY(EditAnywhere, Category = "SkeletalMesh", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SkeletalLegs;
	UPROPERTY(EditAnywhere, Category = "SkeletalMesh", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SkeletalTorso;
	UPROPERTY(EditAnywhere, Category = "SkeletalMesh", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SkeletalFace;

	UPROPERTY(EditAnywhere, Category = "SkeletalMesh", meta = (AllowPrivateAccess = "true"))
	UGroomComponent* SkeletalHair;
	UPROPERTY(EditAnywhere, Category = "SkeletalMesh", meta = (AllowPrivateAccess = "true"))
	UGroomComponent* SkeletalBeard;
	UPROPERTY(EditAnywhere, Category = "SkeletalMesh", meta = (AllowPrivateAccess = "true"))
	UGroomComponent* SkeletalEyebrows;
	UPROPERTY(EditAnywhere, Category = "SkeletalMesh", meta = (AllowPrivateAccess = "true"))
	UGroomComponent* SkeletalEyelashes;
	UPROPERTY(EditAnywhere, Category = "SkeletalMesh", meta = (AllowPrivateAccess = "true"))
	UGroomComponent* SkeletalFuzz;
	UPROPERTY(EditAnywhere, Category = "SkeletalMesh", meta = (AllowPrivateAccess = "true"))
	UGroomComponent* SkeletalMustache;

	UPROPERTY(EditAnywhere,Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	UPROPERTY(EditAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

};
