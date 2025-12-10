// RustyLakeOrreryEditor.Target.cs

using UnrealBuildTool;
using System.Collections.Generic;

public class RustyLakeOrreryEditorTarget : TargetRules
{
	public RustyLakeOrreryEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange(new string[] { "RustyLakeOrrery" });
	}
}
