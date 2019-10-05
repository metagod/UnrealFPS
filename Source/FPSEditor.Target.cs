// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class FPSEditorTarget : TargetRules
{
	public FPSEditorTarget(TargetInfo Target) : base (Target)
	{
		Type = TargetType.Editor;
		ExtraModuleNames.Add ("FPS");
	}

	//
	// TargetRules interface.
	//

	// public override void SetupBinaries(
		// TargetInfo Target,
		// ref List<UEBuildBinaryConfiguration> OutBuildBinaryConfigurations,
		// ref List<string> OutExtraModuleNames
		// )
	// {
		// OutExtraModuleNames.Add("FPS");
	// }
}