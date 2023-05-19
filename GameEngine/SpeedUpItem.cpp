#include "SpeedUpItem.h"

SpeedUpItem::SpeedUpItem(GameObject* parent)
	:ItemBase(parent,"SpeedUpItem"),
	hModel_(-1)
{
}

SpeedUpItem::~SpeedUpItem()
{
}

void SpeedUpItem::Initialize()
{
	//hModel_=
	SetItemType(ITEM_TYPE::UP_SPEED);
}

void SpeedUpItem::Update()
{
}

void SpeedUpItem::Draw()
{
}

void SpeedUpItem::Release()
{
}
