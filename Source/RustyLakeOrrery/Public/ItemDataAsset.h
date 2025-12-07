// ItemDataAsset.h

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemDataAsset.generated.h"

/**
 * @brief 物品类型枚举
 * 
 * 定义游戏中物品的分类，用于UI显示和逻辑判断。
 */
UENUM(BlueprintType)
enum class EItemType : uint8
{
	/** 关键物品：用于推进剧情或解谜 */
	Key UMETA(DisplayName = "Key Item"),
	
	/** 工具：可以多次使用的物品 */
	Tool UMETA(DisplayName = "Tool"),
	
	/** 消耗品：使用后消失 */
	Consumable UMETA(DisplayName = "Consumable"),
	
	/** 收藏品：纯收集用途 */
	Collectible UMETA(DisplayName = "Collectible"),
	
	/** 文档：日记、笔记等可阅读物品 */
	Document UMETA(DisplayName = "Document")
};

/**
 * @brief 物品数据资产类
 * 
 * 定义游戏中每个物品的静态数据。
 * 这是一个纯数据类，不包含运行时逻辑。
 * 
 * 使用方法：
 * 1. 在Content Browser中创建此类的Data Asset
 * 2. 配置物品属性（名称、描述、图标等）
 * 3. 在InteractableComponent中引用此Data Asset
 * 
 * 示例：
 * - DA_Key01: 钥匙物品
 * - DA_Diary_Page1: 日记第一页
 * - DA_Magnifier: 放大镜工具
 */
UCLASS(BlueprintType)
class RUSTYLAKEORRERY_API UItemDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	// ========================================================================
	// 基础属性
	// ========================================================================
	
	/** 物品唯一ID（用于存档和逻辑判断） */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Basic")
	FName ItemID;
	
	/** 物品显示名称 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Basic")
	FText ItemName;
	
	/** 物品详细描述 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Basic", meta = (MultiLine = true))
	FText ItemDescription;
	
	/** 物品类型 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Basic")
	EItemType ItemType = EItemType::Key;
	
	// ========================================================================
	// 视觉资源
	// ========================================================================
	
	/** 物品图标（用于UI显示） */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual")
	UTexture2D* ItemIcon = nullptr;
	
	/** 物品3D模型（可选，用于场景中显示） */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual")
	UStaticMesh* ItemMesh = nullptr;
	
	/** 物品缩略图（用于背包详情显示） */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual")
	UTexture2D* ItemThumbnail = nullptr;
	
	// ========================================================================
	// 游戏逻辑属性
	// ========================================================================
	
	/** 是否可以堆叠（同一物品可以拥有多个） */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	bool bStackable = false;
	
	/** 最大堆叠数量（仅当bStackable=true时有效） */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay", meta = (EditCondition = "bStackable", EditConditionHides, ClampMin = "1"))
	int32 MaxStackSize = 99;
	
	/** 是否可以使用（在背包中点击时是否触发使用事件） */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	bool bUsableFromInventory = false;
	
	/** 使用后是否消耗（从背包移除） */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay", meta = (EditCondition = "bUsableFromInventory", EditConditionHides))
	bool bConsumeOnUse = true;
	
	/** 是否可以丢弃 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	bool bCanBeDropped = false;
	
	// ========================================================================
	// 音效
	// ========================================================================
	
	/** 拾取时播放的音效 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	USoundBase* PickupSound = nullptr;
	
	/** 使用时播放的音效 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	USoundBase* UseSound = nullptr;
	
	/** 丢弃时播放的音效 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	USoundBase* DropSound = nullptr;
	
	// ========================================================================
	// 文档类型专用属性
	// ========================================================================
	
	/** 文档内容（仅当ItemType=Document时使用） */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Document", meta = (EditCondition = "ItemType == EItemType::Document", EditConditionHides, MultiLine = true))
	FText DocumentContent;
	
	/** 文档图片（可选，用于显示插图） */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Document", meta = (EditCondition = "ItemType == EItemType::Document", EditConditionHides))
	UTexture2D* DocumentImage = nullptr;
	
	// ========================================================================
	// 调试和开发
	// ========================================================================
	
	/** 开发备注（不会在游戏中显示） */
	UPROPERTY(EditAnywhere, Category = "Debug", meta = (MultiLine = true))
	FString DeveloperNotes;

public:
	// ========================================================================
	// 公共方法
	// ========================================================================
	
	/**
	 * @brief 获取物品的显示信息
	 * @return 格式化的物品信息文本
	 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	FText GetDisplayInfo() const;
	
	/**
	 * @brief 检查物品ID是否有效
	 * @return 如果ItemID不为None则返回true
	 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	bool IsValid() const;
	
	/**
	 * @brief 获取物品类型的本地化名称
	 * @return 物品类型的显示名称
	 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	FText GetItemTypeName() const;

#if WITH_EDITOR
	/**
	 * @brief 编辑器中的属性变更回调
	 * 用于验证数据完整性
	 */
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
