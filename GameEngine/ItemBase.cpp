#include "ItemBase.h"

ItemBase::ItemBase(GameObject* parent, std::string itemName)
	:GameObject(parent,itemName),
	itemType_(ITEM_TYPE::ITEM_NONE),
	pParticle_(nullptr),
	attacheObject_(nullptr)
{
	SetTag("Item");
	pParticle_ = Instantiate<Particle>(this);
}

ITEM_TYPE ItemBase::GetItemType()
{
	return itemType_;
}

int ItemBase::GetLifeTime()
{
	return lifeTime_;
}

void ItemBase::SubtractTime(int time)
{
	lifeTime_-=time;
}

bool ItemBase::IsAttach()
{
	return isAttached_;
}

void ItemBase::SetCollision()
{
	isCollision_ = true;
}

bool ItemBase::IsCollision()
{
	return isCollision_;
}

void ItemBase::SetDefaultParameter(float param)
{
	defaultParameter_ = param;
}

void ItemBase::SetItemType(ITEM_TYPE type)
{
	itemType_ = type;
}

void ItemBase::SetLifeTime(int time)
{
	SetIniTime(time);
	lifeTime_ = time;
}

void ItemBase::AttachItem(GameObject* target)
{
	attacheObject_ = target;
	isAttached_ = true;
}
