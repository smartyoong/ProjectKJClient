// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Misc/FileHelper.h"
#include "UObject/Class.h"
#include "JsonUtilities.h"
#include "GlobalTypes.h"

/**
 * 
 */
class PROJECTKJCLIENT_API ResourceLoader
{
private:
	FString JsonFilePath;
	void FindJsonFiles(const FString DirectoryPath, TArray<FString>& OutJsonFiles);

	template<typename T>
	T ReadJsonFile(const FString FilePath)
	{
        FString FileContent;
        T Data;

        // 파일 내용을 문자열로 읽기
        if (FFileHelper::LoadFileToString(FileContent, *FilePath))
        {
            TSharedPtr<FJsonObject> JsonObject;
            TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(FileContent);

            // JSON 파싱
            if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
            {
                FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &Data, 0, 0);
            }
        }
        return Data;
	}
public:
	ResourceLoader();
	~ResourceLoader();
	bool PrepareToLoad();
	void LoadChracterPresetInfo(TMap<int32, FJsonChracterPreset>& PresetMap);
    void LoadMapInfo(TMap<int32, FJsonMapInfo>& MapInfoMap);
};
