// InventoryComponent.h

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemDataAsset.h"
#include "InventoryComponent.generated.h"

/**
 * @brief 背包物品槽结构
 * 
 * 存储单个物品槽的信息，包括物品数据和数量。
 */
USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()

	/** 物品数据资产 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UItemDataAsset* ItemData = nullptr;
	
	/** 物品数量 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 Quantity = 1;
	
	/** 默认构造函数 */
	FInventorySlot()
		: ItemData(nullptr)
		, Quantity(1)
	{
	}
	
	/** 带参数构造函数 */
	FInventorySlot(UItemDataAsset* InItemData, int32 InQuantity = 1)
		: ItemData(InItemData)
		, Quantity(InQuantity)
	{
	}
	
	/** 检查槽位是否有效 */
	bool IsValid() const
	{
		return ItemData != nullptr && Quantity > 0;
	}
	
	/** 相等比较（只比较ItemData） */
	bool operator==(const FInventorySlot& Other) const
	{
		return ItemData == Other.ItemData;
	}
};

/**
 * @brief 背包组件类
 * 
 * 管理玩家的物品背包，提供添加、移除、查询物品的功能。
 * 
 * 设计原则：
 * - 高度集成的黑盒组件
 * - 自动处理物品堆叠
 * - 自动触发UI更新事件
 * - 为SaveSystem预留序列化接口
 * 
 * 使用方法：
 * 1. 将此组件添加到PlayerController蓝图
 * 2. 配置背包容量（可选）
 * 3. 使用AddItem/RemoveItem/HasItem等函数管理物品
 * 4. 监听OnInventoryUpdated事件刷新UI
 * 
 * 示例：
 * ```cpp
 * // 在InteractableComponent中拾取物品
 * UInventoryComponent* Inventory = PlayerController->FindComponentByClass<UInventoryComponent>();
 * if (Inventory)
 * {
 *     Inventory->AddItem(PickupItemData);
 * }
 * ```
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RUSTYLAKEORRERY_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// ========================================================================
	// 构造函数和生命周期
	// ========================================================================
	
	UInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:
	// ========================================================================
	// 配置属性
	// ========================================================================
	
	/** 背包最大容量（0表示无限制） */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Settings")
	int32 MaxCapacity = 20;
	
	/** 是否允许自动堆叠物品 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Settings")
	bool bAutoStack = true;
	
	/** 是否在拾取时播放音效 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Settings")
	bool bPlayPickupSound = true;
	
	/** 是否在使用时播放音效 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Settings")
	bool bPlayUseSound = true;
	
	/** 默认拾取音效（如果物品没有自定义音效） */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Settings")
	USoundBase* DefaultPickupSound = nullptr;
	
	/** 默认使用音效（如果物品没有自定义音效） */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Settings")
	USoundBase* DefaultUseSound = nullptr;
	
	// ========================================================================
	// 背包数据
	// ========================================================================
	
private:
	/** 背包物品槽数组 */
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<FInventorySlot> InventorySlots;

public:
	// ========================================================================
	// 核心API
	// ========================================================================
	
	/**
	 * @brief 添加物品到背包
	 * @param ItemData 要添加的物品数据
	 * @param Quantity 添加数量（默认1）
	 * @return 是否成功添加
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddItem(UItemDataAsset* ItemData, int32 Quantity = 1);
	
	/**
	 * @brief 从背包移除物品
	 * @param ItemData 要移除的物品数据
	 * @param Quantity 移除数量（默认1）
	 * @return 是否成功移除
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItem(UItemDataAsset* ItemData, int32 Quantity = 1);
	
	/**
	 * @brief 检查背包中是否有指定物品
	 * @param ItemData 要检查的物品数据
	 * @param RequiredQuantity 需要的数量（默认1）
	 * @return 是否拥有足够数量的物品
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	bool HasItem(UItemDataAsset* ItemData, int32 RequiredQuantity = 1) const;
	
	/**
	 * @brief 获取指定物品的数量
	 * @param ItemData 要查询的物品数据
	 * @return 物品数量（如果没有则返回0）
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	int32 GetItemQuantity(UItemDataAsset* ItemData) const;
	
	/**
	 * @brief 使用背包中的物品
	 * @param ItemData 要使用的物品数据
	 * @return 是否成功使用
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool UseItem(UItemDataAsset* ItemData);
	
	/**
	 * @brief 清空背包
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ClearInventory();
	
	/**
	 * @brief 获取所有物品槽
	 * @return 物品槽数组的只读引用
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	const TArray<FInventorySlot>& GetAllItems() const { return InventorySlots; }
	
	/**
	 * @brief 获取当前物品数量
	 * @return 背包中不同物品的种类数
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	int32 GetItemCount() const { return InventorySlots.Num(); }
	
	/**
	 * @brief 检查背包是否已满
	 * @return 如果背包已满则返回true
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	bool IsFull() const;
	
	/**
	 * @brief 检查背包是否为空
	 * @return 如果背包为空则返回true
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	bool IsEmpty() const { return InventorySlots.Num() == 0; }
	
	// ========================================================================
	// 高级API
	// ========================================================================
	
	/**
	 * @brief 按类型获取物品列表
	 * @param ItemType 物品类型
	 * @return 指定类型的物品槽数组
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<FInventorySlot> GetItemsByType(EItemType ItemType) const;
	
	/**
	 * @brief 通过ItemID查找物品
	 * @param ItemID 物品ID
	 * @return 找到的物品数据（如果没有则返回nullptr）
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	UItemDataAsset* FindItemByID(FName ItemID) const;
	
	/**
	 * @brief 交换两个物品槽的位置
	 * @param IndexA 第一个槽位索引
	 * @param IndexB 第二个槽位索引
	 * @return 是否成功交换
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool SwapItems(int32 IndexA, int32 IndexB);
	
	/**
	 * @brief 对背包物品进行排序
	 * @param bSortByType 是否按类型排序（否则按名称排序）
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SortInventory(bool bSortByType = true);
	
	// ========================================================================
	// 事件委托
	// ========================================================================
	
	/** 当背包内容更新时广播（添加/移除/使用物品） */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);
	UPROPERTY(BlueprintAssignable, Category = "Inventory Events")
	FOnInventoryUpdated OnInventoryUpdated;
	
	/** 当添加物品时广播 */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemAdded, UItemDataAsset*, ItemData, int32, Quantity);
	UPROPERTY(BlueprintAssignable, Category = "Inventory Events")
	FOnItemAdded OnItemAdded;
	
	/** 当移除物品时广播 */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemRemoved, UItemDataAsset*, ItemData, int32, Quantity);
	UPROPERTY(BlueprintAssignable, Category = "Inventory Events")
	FOnItemRemoved OnItemRemoved;
	
	/** 当使用物品时广播 */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemUsed, UItemDataAsset*, ItemData);
	UPROPERTY(BlueprintAssignable, Category = "Inventory Events")
	FOnItemUsed OnItemUsed;
	
	/** 当背包已满无法添加物品时广播 */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryFull, UItemDataAsset*, ItemData);
	UPROPERTY(BlueprintAssignable, Category = "Inventory Events")
	FOnInventoryFull OnInventoryFull;
	
	// ========================================================================
	// 调试功能
	// ========================================================================
	
	/** 是否在屏幕上显示调试信息 */
	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bShowDebugInfo = false;
	
	/**
	 * @brief 打印背包内容到日志
	 */
	UFUNCTION(BlueprintCallable, Category = "Debug")
	void PrintInventoryToLog() const;

private:
	// ========================================================================
	// 内部辅助函数
	// ========================================================================
	
	/**
	 * @brief 查找物品槽索引
	 * @param ItemData 要查找的物品数据
	 * @return 槽位索引（如果没有则返回-1）
	 */
	int32 FindSlotIndex(UItemDataAsset* ItemData) const;
	
	/**
	 * @brief 播放物品音效
	 * @param Sound 要播放的音效
	 */
	void PlayItemSound(USoundBase* Sound);
	
	/**
	 * @brief 广播背包更新事件
	 */
	void BroadcastInventoryUpdated();
};
