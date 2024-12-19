// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using UnrealBuildTool.Rules;

public class ProjectKJClient : ModuleRules
{
	public ProjectKJClient(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput", "Networking", "Sockets",

        "MediaAssets","AudioExtensions","UMG","Slate","SlateCore","Json","JsonUtilities","HairStrandsCore","Niagara"});

        PrivateIncludePaths.Add("ProjectKJClient");
        PrivateIncludePaths.Add("ProjectKJClient/SocketCore");
        PrivateIncludePaths.Add("ProjectKJClient/ThreadCore");
        PrivateIncludePaths.Add("ProjectKJClient/UtilityLibrary");
        PrivateIncludePaths.Add("ProjectKJClient/WidgetUI");
        PrivateIncludePaths.Add("ProjectKJClient/PacketList");
        PrivateIncludePaths.Add("ProjectKJClient/MapObject");
        PrivateIncludePaths.Add("ProjectKJClient/Global");
        PrivateIncludePaths.Add("ProjectKJClient/GameMode");
        PrivateIncludePaths.Add("ProjectKJClient/Controller");
        PrivateIncludePaths.Add("ProjectKJClient/Character");
        PrivateIncludePaths.Add("ProjectKJClient/Component");
        PrivateIncludePaths.Add("ProjectKJClient/Pawns");
    }
}
