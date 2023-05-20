#include "ItemGetter.h"
#include"SpeedUpItem.h"
#include"Player.h"
ItemGetter::ItemGetter(GameObject* object)
	:attachObject_(object)
{
}

ItemGetter::~ItemGetter()
{
}

void ItemGetter::Update()
{
	for (auto itr=itemList_.begin();itr!=itemList_.end();)
	{

		if ((*itr)->GetLifeTime() == 0)
		{
			RemoveItemEffect((*itr));
			itr = itemList_.erase(itr);
		}
		else
			itr++;
	}
}

void ItemGetter::ItemAttach(ItemBase* item)
{	
	item->AttachItem();
	Apply(item);
	itemList_.push_back(item);
}

void ItemGetter::ItemRemove(ItemBase* item)
{
	RemoveItemEffect(item);
}

void ItemGetter::Apply(ItemBase* item)
{
	switch (item->GetItemType())
	{
	case ITEM_TYPE::UP_SPEED:
		item->SetDefaultParameter(((Player*)attachObject_)->GetSpeed());
		((Player*)attachObject_)->SetSpeed(((SpeedUpItem*)item)->GetItemParam());
		break;
	case ITEM_TYPE::UP_SEARCH:
		break;
	default:
		break;
	}
}

void ItemGetter::RemoveItemEffect(ItemBase* item)
{
	switch (item->GetItemType())
	{
	case ITEM_TYPE::UP_SPEED:
		((Player*)attachObject_)->SetSpeed(item->GetDefaultParameter());
		break;
	case ITEM_TYPE::UP_SEARCH:
		break;
	default:
		break;
	}
}
