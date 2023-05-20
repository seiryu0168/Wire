#include "ItemGetter.h"
#include"SpeedUpItem.h"
#include"SearchUpItem.h"
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
			(*itr)->KillMe();
			itr = itemList_.erase(itr);
		}
		else
			itr++;
	}
}

void ItemGetter::ItemAttach(ItemBase* item)
{	
	//�����A�C�e�������Ɏ擾���Ă�����
	if (CheckSameItem(item->GetItemType()))
	{
		return;
	}
		//�A�C�e���̌��ʂ�L���ɂ���
		item->AttachItem();
		Apply(item);
		//�A�C�e�����X�g�ɒǉ�
		itemList_.push_back(item);
}

void ItemGetter::ItemRemove(ItemBase* item)
{
	//���ʂ�����
	RemoveItemEffect(item);
}

void ItemGetter::Apply(ItemBase* item)
{
	switch (item->GetItemType())
	{
	case ITEM_TYPE::SPEED:
		item->SetDefaultParameter(((Player*)attachObject_)->GetSpeed());
		((Player*)attachObject_)->SetSpeed(((SpeedUpItem*)item)->GetItemParam());
		break;
	case ITEM_TYPE::SEARCH:
		item->SetDefaultParameter(((Player*)attachObject_)->GetLockOnAngleLimit());
		((Player*)attachObject_)->SetLockOnAngleLimit(((SearchUpItem*)item)->GetItemParam());
		break;
	default:
		break;
	}
}

bool ItemGetter::CheckSameItem(ITEM_TYPE type)
{
	for (auto i : itemList_)
	{
		if (i->GetItemType() == type)
			return true;
	}
	return false;
}

void ItemGetter::RemoveItemEffect(ItemBase* item)
{
	switch (item->GetItemType())
	{
	case ITEM_TYPE::SPEED:
		((Player*)attachObject_)->SetSpeed(item->GetDefaultParameter());
		break;
	case ITEM_TYPE::SEARCH:
		((Player*)attachObject_)->SetLockOnAngleLimit(item->GetDefaultParameter());
		break;
	default:
		break;
	}
}
