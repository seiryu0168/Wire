#include "ItemBase.h"

ItemBase::ItemBase(GameObject* parent, std::string itemName)
	:GameObject(parent,itemName),
	itemType_(ITEM_TYPE::ITEM_NONE)
{
	SetTag("Item");
}

ITEM_TYPE ItemBase::GetItemType()
{
	return itemType_;
}

int ItemBase::GetLifeTime()
{
	return lifeTime_;
}

void ItemBase::SetItemType(ITEM_TYPE type)
{
	itemType_ = type;
}

void ItemBase::SetLifeTime(int time)
{
	lifeTime_ = time;
}
