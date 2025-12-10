// InventoryComponent.cpp

#include "InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// 初始化背包
	InventorySlots.Empty();
	
	if (bShowDebugInfo)
	{
		UE_LOG(LogTemp, Log, TEXT("[InventoryComponent] Initialized with MaxCapacity=%d"), MaxCapacity);
	}
}

bool UInventoryComponent::AddItem(UItemDataAsset* ItemData, int32 Quantity)
{
	// 验证输入
	if (!ItemData || Quantity <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("[InventoryComponent] AddItem failed: Invalid ItemData or Quantity"));
		return false;
	}
	
	if (!ItemData->IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("[InventoryComponent] AddItem failed: ItemData is not valid"));
		return false;
	}
	
	// 检查背包是否已满
	if (IsFull() && !HasItem(ItemData))
	{
		UE_LOG(LogTemp, Warning, TEXT("[InventoryComponent] Inventory is full, cannot add %s"), *ItemData->ItemName.ToString());
		OnInventoryFull.Broadcast(ItemData);
		return false;
	}
	
	// 查找现有槽位
	int32 SlotIndex = FindSlotIndex(ItemData);
	
	if (SlotIndex != INDEX_NONE)
	{
		// 物品已存在，检查是否可堆叠
		if (bAutoStack && ItemData->bStackable)
		{
			FInventorySlot& Slot = InventorySlots[SlotIndex];
			int32 NewQuantity = Slot.Quantity + Quantity;
			
			// 检查是否超过最大堆叠数量
			if (NewQuantity > ItemData->MaxStackSize)
			{
				Slot.Quantity = ItemData->MaxStackSize;
				UE_LOG(LogTemp, Warning, TEXT("[InventoryComponent] Item %s reached max stack size %d"), 
					*ItemData->ItemName.ToString(), ItemData->MaxStackSize);
			}
			else
			{
				Slot.Quantity = NewQuantity;
			}
		}
		else
		{
			// 不可堆叠，无法添加更多
			UE_LOG(LogTemp, Warning, TEXT("[InventoryComponent] Item %s is not stackable"), *ItemData->ItemName.ToString());
			return false;
		}
	}
	else
	{
		// 创建新槽位
		FInventorySlot NewSlot(ItemData, Quantity);
		
		// 检查堆叠数量限制
		if (ItemData->bStackable && NewSlot.Quantity > ItemData->MaxStackSize)
		{
			NewSlot.Quantity = ItemData->MaxStackSize;
		}
		
		InventorySlots.Add(NewSlot);
	}
	
	// 播放拾取音效
	if (bPlayPickupSound)
	{
		USoundBase* SoundToPlay = ItemData->PickupSound ? ItemData->PickupSound : DefaultPickupSound;
		PlayItemSound(SoundToPlay);
	}
	
	// 广播事件
	OnItemAdded.Broadcast(ItemData, Quantity);
	BroadcastInventoryUpdated();
	
	UE_LOG(LogTemp, Log, TEXT("[InventoryComponent] Added %d x %s"), Quantity, *ItemData->ItemName.ToString());
	
	return true;
}

bool UInventoryComponent::RemoveItem(UItemDataAsset* ItemData, int32 Quantity)
{
	// 验证输入
	if (!ItemData || Quantity <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("[InventoryComponent] RemoveItem failed: Invalid ItemData or Quantity"));
		return false;
	}
	
	// 查找槽位
	int32 SlotIndex = FindSlotIndex(ItemData);
	if (SlotIndex == INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("[InventoryComponent] RemoveItem failed: Item %s not found"), *ItemData->ItemName.ToString());
		return false;
	}
	
	FInventorySlot& Slot = InventorySlots[SlotIndex];
	
	// 检查数量是否足够
	if (Slot.Quantity < Quantity)
	{
		UE_LOG(LogTemp, Warning, TEXT("[InventoryComponent] RemoveItem failed: Not enough quantity (has %d, need %d)"), 
			Slot.Quantity, Quantity);
		return false;
	}
	
	// 减少数量
	Slot.Quantity -= Quantity;
	
	// 如果数量为0，移除槽位
	if (Slot.Quantity <= 0)
	{
		InventorySlots.RemoveAt(SlotIndex);
	}
	
	// 广播事件
	OnItemRemoved.Broadcast(ItemData, Quantity);
	BroadcastInventoryUpdated();
	
	UE_LOG(LogTemp, Log, TEXT("[InventoryComponent] Removed %d x %s"), Quantity, *ItemData->ItemName.ToString());
	
	return true;
}

bool UInventoryComponent::HasItem(UItemDataAsset* ItemData, int32 RequiredQuantity) const
{
	if (!ItemData || RequiredQuantity <= 0)
	{
		return false;
	}
	
	int32 SlotIndex = FindSlotIndex(ItemData);
	if (SlotIndex == INDEX_NONE)
	{
		return false;
	}
	
	return InventorySlots[SlotIndex].Quantity >= RequiredQuantity;
}

int32 UInventoryComponent::GetItemQuantity(UItemDataAsset* ItemData) const
{
	if (!ItemData)
	{
		return 0;
	}
	
	int32 SlotIndex = FindSlotIndex(ItemData);
	if (SlotIndex == INDEX_NONE)
	{
		return 0;
	}
	
	return InventorySlots[SlotIndex].Quantity;
}

bool UInventoryComponent::UseItem(UItemDataAsset* ItemData)
{
	// 验证输入
	if (!ItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("[InventoryComponent] UseItem failed: Invalid ItemData"));
		return false;
	}
	
	// 检查物品是否可使用
	if (!ItemData->bUsableFromInventory)
	{
		UE_LOG(LogTemp, Warning, TEXT("[InventoryComponent] Item %s is not usable from inventory"), *ItemData->ItemName.ToString());
		return false;
	}
	
	// 检查是否拥有该物品
	if (!HasItem(ItemData))
	{
		UE_LOG(LogTemp, Warning, TEXT("[InventoryComponent] UseItem failed: Don't have %s"), *ItemData->ItemName.ToString());
		return false;
	}
	
	// 播放使用音效
	if (bPlayUseSound)
	{
		USoundBase* SoundToPlay = ItemData->UseSound ? ItemData->UseSound : DefaultUseSound;
		PlayItemSound(SoundToPlay);
	}
	
	// 广播使用事件
	OnItemUsed.Broadcast(ItemData);
	
	// 如果使用后消耗，则移除物品
	if (ItemData->bConsumeOnUse)
	{
		RemoveItem(ItemData, 1);
	}
	
	UE_LOG(LogTemp, Log, TEXT("[InventoryComponent] Used item: %s"), *ItemData->ItemName.ToString());
	
	return true;
}

void UInventoryComponent::ClearInventory()
{
	InventorySlots.Empty();
	BroadcastInventoryUpdated();
	
	UE_LOG(LogTemp, Log, TEXT("[InventoryComponent] Inventory cleared"));
}

bool UInventoryComponent::IsFull() const
{
	if (MaxCapacity <= 0)
	{
		return false; // 无限容量
	}
	
	return InventorySlots.Num() >= MaxCapacity;
}

TArray<FInventorySlot> UInventoryComponent::GetItemsByType(EItemType ItemType) const
{
	TArray<FInventorySlot> Result;
	
	for (const FInventorySlot& Slot : InventorySlots)
	{
		if (Slot.ItemData && Slot.ItemData->ItemType == ItemType)
		{
			Result.Add(Slot);
		}
	}
	
	return Result;
}

UItemDataAsset* UInventoryComponent::FindItemByID(FName ItemID) const
{
	for (const FInventorySlot& Slot : InventorySlots)
	{
		if (Slot.ItemData && Slot.ItemData->ItemID == ItemID)
		{
			return Slot.ItemData;
		}
	}
	
	return nullptr;
}

bool UInventoryComponent::SwapItems(int32 IndexA, int32 IndexB)
{
	// 验证索引
	if (!InventorySlots.IsValidIndex(IndexA) || !InventorySlots.IsValidIndex(IndexB))
	{
		UE_LOG(LogTemp, Warning, TEXT("[InventoryComponent] SwapItems failed: Invalid indices"));
		return false;
	}
	
	// 交换
	InventorySlots.Swap(IndexA, IndexB);
	BroadcastInventoryUpdated();
	
	return true;
}

void UInventoryComponent::SortInventory(bool bSortByType)
{
	if (bSortByType)
	{
		// 按类型排序
		InventorySlots.Sort([](const FInventorySlot& A, const FInventorySlot& B)
		{
			if (!A.ItemData || !B.ItemData) return false;
			
			// 先按类型排序
			if (A.ItemData->ItemType != B.ItemData->ItemType)
			{
				return A.ItemData->ItemType < B.ItemData->ItemType;
			}
			
			// 类型相同则按名称排序
			return A.ItemData->ItemName.CompareTo(B.ItemData->ItemName) < 0;
		});
	}
	else
	{
		// 按名称排序
		InventorySlots.Sort([](const FInventorySlot& A, const FInventorySlot& B)
		{
			if (!A.ItemData || !B.ItemData) return false;
			return A.ItemData->ItemName.CompareTo(B.ItemData->ItemName) < 0;
		});
	}
	
	BroadcastInventoryUpdated();
	
	UE_LOG(LogTemp, Log, TEXT("[InventoryComponent] Inventory sorted by %s"), bSortByType ? TEXT("type") : TEXT("name"));
}

void UInventoryComponent::PrintInventoryToLog() const
{
	UE_LOG(LogTemp, Log, TEXT("========== Inventory Contents =========="));
	UE_LOG(LogTemp, Log, TEXT("Capacity: %d/%d"), InventorySlots.Num(), MaxCapacity > 0 ? MaxCapacity : 999);
	
	if (InventorySlots.Num() == 0)
	{
		UE_LOG(LogTemp, Log, TEXT("(Empty)"));
	}
	else
	{
		for (int32 i = 0; i < InventorySlots.Num(); ++i)
		{
			const FInventorySlot& Slot = InventorySlots[i];
			if (Slot.ItemData)
			{
				UE_LOG(LogTemp, Log, TEXT("[%d] %s x%d (ID: %s, Type: %s)"), 
					i,
					*Slot.ItemData->ItemName.ToString(),
					Slot.Quantity,
					*Slot.ItemData->ItemID.ToString(),
					*Slot.ItemData->GetItemTypeName().ToString()
				);
			}
		}
	}
	
	UE_LOG(LogTemp, Log, TEXT("========================================"));
}

int32 UInventoryComponent::FindSlotIndex(UItemDataAsset* ItemData) const
{
	if (!ItemData)
	{
		return INDEX_NONE;
	}
	
	for (int32 i = 0; i < InventorySlots.Num(); ++i)
	{
		if (InventorySlots[i].ItemData == ItemData)
		{
			return i;
		}
	}
	
	return INDEX_NONE;
}

void UInventoryComponent::PlayItemSound(USoundBase* Sound)
{
	if (!Sound)
	{
		return;
	}
	
	// 获取PlayerController
	APlayerController* PC = Cast<APlayerController>(GetOwner());
	if (!PC)
	{
		return;
	}
	
	// 播放2D音效
	UGameplayStatics::PlaySound2D(GetWorld(), Sound);
}

void UInventoryComponent::BroadcastInventoryUpdated()
{
	OnInventoryUpdated.Broadcast();
	
	if (bShowDebugInfo)
	{
		PrintInventoryToLog();
	}
}
