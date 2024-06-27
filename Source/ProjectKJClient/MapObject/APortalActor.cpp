// Fill out your copyright notice in the Description page of Project Settings.


#include "APortalActor.h"
#include "Components/BoxComponent.h"
#include "MainGameInstance.h"

// Sets default values
AAPortalActor::AAPortalActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;
	BoxComponent->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f));
	BoxComponent->SetCollisionProfileName(TEXT("Trigger"));
	//BoxComponent->SetGenerateOverlapEvents(true); 이건 굳이 필요 없을듯
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AAPortalActor::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AAPortalActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAPortalActor::GetPortalBoxExtent(FVector& OutExtent) const
{
	OutExtent = BoxComponent->GetScaledBoxExtent();
}

// Called every frame
void AAPortalActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAPortalActor::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 플레이어와 겹쳤을 때 다른 맵으로 이동시킵니다.
	// 포탈에 겹쳐있다는걸 알려준다. 그리고 이동키를 누르면 이동 시킨다.
	//if (OtherActor->IsA(ACharacter::StaticClass()))
	//{
	//	IsOnPortal = true;
	//}
}

void AAPortalActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 플레이어와 겹쳐있지 않을 때 다른 맵으로 이동시키는 것을 취소합니다.
	// 포탈에 겹쳐있지 않다는걸 알려준다. 그리고 이동키를 누르면 이동을 취소한다.
	//if (OtherActor->IsA(ACharacter::StaticClass()))
	//{
	//	IsOnPortal = false;
	//}
}

