// ItemDataAsset.cpp

#include "ItemDataAsset.h"

FText UItemDataAsset::GetDisplayInfo() const
{
	if (!IsValid())
	{
		return FText::FromString(TEXT("Invalid Item"));
	}
	
	// 格式化显示信息：[类型] 名称\n描述
	FString TypeName = GetItemTypeName().ToString();
	FString DisplayText = FString::Printf(TEXT("[%s] %s\n\n%s"), 
		*TypeName, 
		*ItemName.ToString(), 
		*ItemDescription.ToString()
	);
	
	return FText::FromString(DisplayText);
}

bool UItemDataAsset::IsValid() const
{
	return !ItemID.IsNone() && !ItemName.IsEmpty();
}

FText UItemDataAsset::GetItemTypeName() const
{
	switch (ItemType)
	{
		case EItemType::Key:
			return FText::FromString(TEXT("关键物品"));
		case EItemType::Tool:
			return FText::FromString(TEXT("工具"));
		case EItemType::Consumable:
			return FText::FromString(TEXT("消耗品"));
		case EItemType::Collectible:
			return FText::FromString(TEXT("收藏品"));
		case EItemType::Document:
			return FText::FromString(TEXT("文档"));
		default:
			return FText::FromString(TEXT("未知"));
	}
}

#if WITH_EDITOR
void UItemDataAsset::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	FName PropertyName = PropertyChangedEvent.GetPropertyName();
	
	// 自动验证：如果ItemID为空，尝试从资产名称生成
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UItemDataAsset, ItemID))
	{
		if (ItemID.IsNone())
		{
			// 从资产名称生成ItemID（移除DA_前缀）
			FString AssetName = GetName();
			if (AssetName.StartsWith(TEXT("DA_")))
			{
				AssetName.RemoveFromStart(TEXT("DA_"));
			}
			ItemID = FName(*AssetName);
		}
	}
	
	// 验证堆叠设置
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UItemDataAsset, bStackable))
	{
		if (!bStackable)
		{
			MaxStackSize = 1;
		}
		else if (MaxStackSize < 1)
		{
			MaxStackSize = 99;
		}
	}
	
	// 验证最大堆叠数量
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UItemDataAsset, MaxStackSize))
	{
		if (MaxStackSize < 1)
		{
			MaxStackSize = 1;
		}
	}
	
	// 文档类型自动设置为可使用
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UItemDataAsset, ItemType))
	{
		if (ItemType == EItemType::Document)
		{
			bUsableFromInventory = true;
			bConsumeOnUse = false;
		}
	}
}
#endif
