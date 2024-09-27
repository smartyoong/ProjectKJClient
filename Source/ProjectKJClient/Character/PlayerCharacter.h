// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GroomComponent.h"
#include "GlobalTypes.h"
#include "KinematicController.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class PROJECTKJCLIENT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();
private:

	//유저 관련 변수
	FString AccountID;
	FString NickName;
	FString AuthHashCode;
	int32 CharacterPresetID = 0;

	//외형 정보
	int32 Gender;

	//캐릭터 정보
	int32 Level;
	int32 EXP;
	int32 Job;
	int32 JobLevel;

	//이동 관련 변수
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivate))
	int32 Speed;
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivate))
	float DestinationBoardRadius;
	bool PlayMovingAnimation;
	int32 CurrentMapID;
	FVector OldLocation;

	KinematicController* KinematicMover;

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


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SetDestinationClickAction;

	UPROPERTY(EditAnywhere, Category = Input)
	class UNiagaraSystem* FXCursor;

	FTimerHandle TimerHandle;
private:
	void PingCheck();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual ~APlayerCharacter();

	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	void SetSpawnBaseInfo(FCharacterInfo Info);
	void MoveToLocation(FVector Location);
	void RollBackLocation();
	void ClickAndMove();
	UFUNCTION(BlueprintCallable)
	bool IsMoveingNow() { return PlayMovingAnimation; };
	int32 GetMapID() { return CurrentMapID; }
	FString GetAccountID() { return AccountID; }
	void SetSpeed(int32 NewSpeed) { Speed = NewSpeed; }
	void PlayMovingAnim(bool NeedPlay) { PlayMovingAnimation = NeedPlay; };
};
