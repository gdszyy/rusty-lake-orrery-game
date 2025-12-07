// RustyLakeOrrery.Target.cs

using UnrealBuildTool;
using System.Collections.Generic;

public class RustyLakeOrreryTarget : TargetRules
{
	public RustyLakeOrreryTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange(new string[] { "RustyLakeOrrery" });
	}
}
