// RLOPlayerController.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RLOPlayerController.generated.h"

/**
 * @brief 玩家控制器基类
 * 
 * 负责处理玩家输入和交互系统。
 * InteractionComponent应该附加到此类的蓝图子类上。
 */
UCLASS()
class RUSTYLAKEORRERY_API ARLOPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ARLOPlayerController();

protected:
	virtual void BeginPlay() override;
};
