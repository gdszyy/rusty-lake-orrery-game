// RLOGameMode.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RLOGameMode.generated.h"

/**
 * @brief 游戏模式基类
 * 
 * Rusty Lake Orrery的主游戏模式，负责管理游戏流程和规则。
 */
UCLASS()
class RUSTYLAKEORRERY_API ARLOGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARLOGameMode();
};
