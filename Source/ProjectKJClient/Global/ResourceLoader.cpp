// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/ResourceLoader.h"
#include "Misc/FileHelper.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "GlobalTypes.h"

void ResourceLoader::FindJsonFiles(const FString DirectoryPath, TArray<FString>& OutJsonFiles)
{
    FString FilePattern = DirectoryPath + TEXT("*.json");
    // 파일 목록을 저장할 임시 배열
    TArray<FString> FoundFiles;

    // 지정된 패턴에 맞는 파일 찾기
    IFileManager::Get().FindFiles(FoundFiles, *FilePattern, true, false);

    // 찾은 파일 경로를 완성하여 결과 배열에 추가
    for (const FString& File : FoundFiles)
    {
        OutJsonFiles.Add(DirectoryPath + File);
    }
}

ResourceLoader::ResourceLoader()
{
}

ResourceLoader::~ResourceLoader()
{
}

bool ResourceLoader::PrepareToLoad()
{
	JsonFilePath = FPaths::ProjectContentDir() + TEXT("Resource/");
    if (IFileManager::Get().DirectoryExists(*JsonFilePath))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void ResourceLoader::LoadChracterPresetInfo(TMap<int32, FJsonChracterPreset>& PresetMap)
{
    TArray<FString> JsonFiles;
    FString ChracterPresetJsonPath = JsonFilePath + TEXT("CharacterPreset/");
    FindJsonFiles(ChracterPresetJsonPath, JsonFiles);
    for (auto FilePaths : JsonFiles)
    {
        FJsonChracterPreset Preset = ReadJsonFile<FJsonChracterPreset>(FilePaths);
		PresetMap.Add(Preset.PresetID,Preset);
    }
}
