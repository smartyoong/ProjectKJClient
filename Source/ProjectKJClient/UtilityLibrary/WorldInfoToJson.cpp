// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldInfoToJson.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMeshActor.h"
#include "APortalActor.h"

WorldInfoToJson::WorldInfoToJson()
{
}

WorldInfoToJson::~WorldInfoToJson()
{
}

void WorldInfoToJson::SaveWorldInfoToJson(UWorld* World, int MapID)
{
    if (World == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("Save Fail : World is nullptr"));
        return;
    }
    FString MapName = World->GetMapName();
    FString OutputString = FString::Printf(TEXT("{\n\t\"MapID\": %d,\n\t\"MapName\": \"%s\",\n\t\"Obstacles\": [\n"), MapID, *MapName);

    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(World, AStaticMeshActor::StaticClass(), FoundActors);

    for (AActor* Actor : FoundActors)
    {
        AStaticMeshActor* MeshActor = Cast<AStaticMeshActor>(Actor);

        if (MeshActor == nullptr)
        {
            continue;
        }

        UStaticMeshComponent* MeshComponent = MeshActor->GetStaticMeshComponent();

        FTransform Transform = Actor->GetActorTransform();
        FVector Location = Transform.GetLocation();
        FVector Scale = Transform.GetScale3D();
        FRotator Rotation = Transform.GetRotation().Rotator();
        FBox BoundingBox = MeshComponent->GetStaticMesh()->GetBoundingBox();
        FVector MeshSize = BoundingBox.GetSize();

        FString MeshName = MeshComponent->GetStaticMesh()->GetName();

        // 실린더의 반지름과 높이 계산
        float CylinderRadius = MeshSize.X * 0.5f * Scale.X;
        float CylinderHeight = MeshSize.Z * Scale.Z;

        // 구의 반지름 계산
        float SphereRadius = FMath::Max3(MeshSize.X, MeshSize.Y, MeshSize.Z) * 0.5f * FMath::Max3(Scale.X, Scale.Y, Scale.Z);

        OutputString += FString::Printf(TEXT("\t\t{\"Location\": {\"X\": %f, \"Y\": %f, \"Z\": %f}, \"Rotation\": {\"Pitch\": %f, \"Yaw\": %f, \"Roll\": %f}, \"Scale\": {\"X\": %f, \"Y\": %f, \"Z\": %f}, \"MeshSize\": {\"X\": %f, \"Y\": %f, \"Z\": %f}, \"MeshName\": \"%s\", \"CylinderRadius\": %f, \"CylinderHeight\": %f, \"SphereRadius\": %f},\n"),
            Location.X, Location.Y, Location.Z, Rotation.Pitch, Rotation.Yaw, Rotation.Roll, Scale.X, Scale.Y, Scale.Z, MeshSize.X, MeshSize.Y, MeshSize.Z, *MeshName, CylinderRadius, CylinderHeight, SphereRadius);
    }
    OutputString.RemoveFromEnd(",\n");
    OutputString += "\n\t]\n}";

    FFileHelper::SaveStringToFile(OutputString, *(FPaths::ProjectDir() + "/MapFiles/" + MapName + ".json"));
}

void WorldInfoToJson::SaveWorldPortalInfoToJson(UWorld* World, int MapID)
{
    if (World == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("Save Fail : World is nullptr"));
        return;
    }
    FString MapName = World->GetMapName();
    FString OutputString = FString::Printf(TEXT("{\n\t\"MapID\": %d,\n\t\"MapName\": \"%s\",\n\t\"Portals\": [\n"), MapID, *MapName);

    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(World, AAPortalActor::StaticClass(), FoundActors);

    for (AActor* Actor : FoundActors)
    {
        AAPortalActor* PortalActor = Cast<AAPortalActor>(Actor);

        if (PortalActor == nullptr)
        {
            continue;
        }

        FTransform Transform = Actor->GetActorTransform();
        FVector Location = Transform.GetLocation();
        FVector Scale = Transform.GetScale3D();
        FRotator Rotation = Transform.GetRotation().Rotator();
        FVector BoxComponentSize;
        PortalActor->GetPortalBoxExtent(BoxComponentSize);
        int32 TargetMapID = PortalActor->GetTargetMapID();

        OutputString += FString::Printf(TEXT("\t\t{\"Location\": {\"X\": %f, \"Y\": %f, \"Z\": %f}, \"Rotation\": {\"Pitch\": %f, \"Yaw\": %f, \"Roll\": %f}, \"Scale\": {\"X\": %f, \"Y\": %f, \"Z\": %f}, \"BoxSize\": {\"X\": %f, \"Y\": %f, \"Z\": %f}, \"LinkMapID\": %d},\n"),
            Location.X, Location.Y, Location.Z, Rotation.Pitch, Rotation.Yaw, Rotation.Roll, Scale.X, Scale.Y, Scale.Z, BoxComponentSize.X, BoxComponentSize.Y, BoxComponentSize.Z, TargetMapID);
    }
    OutputString.RemoveFromEnd(",\n");
    OutputString += "\n\t]\n}";
    FFileHelper::SaveStringToFile(OutputString, *(FPaths::ProjectDir() + "/MapLinkFiles/" + MapName + "Link.json"));
}
