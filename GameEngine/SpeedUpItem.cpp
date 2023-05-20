#include "SpeedUpItem.h"
#include"Engine/Collider/BoxCollider.h"
namespace
{
	static const float SPEED = 1.3f;
	static const int LIFE = 600;
}
SpeedUpItem::SpeedUpItem(GameObject* parent)
	:ItemBase(parent,"SpeedUpItem"),
	hModel_(-1)
{
	value_ = SPEED;
}

SpeedUpItem::~SpeedUpItem()
{
}

void SpeedUpItem::Initialize()
{
	SetLifeTime(LIFE);
	BoxCollider* pCollision = new BoxCollider({ 0,0,0 }, { 1,1,1 });
	AddCollider(pCollision);
	hModel_ = ModelManager::Load("Assets\\AAA.fbx");
	SetItemType(ITEM_TYPE::UP_SPEED);
}

void SpeedUpItem::Update()
{
	if(IsAttach())
	{
		SubtractTime();
	}
}

void SpeedUpItem::Draw()
{
	ModelManager::SetTransform(hModel_, transform_);
	ModelManager::Draw(hModel_);
}

void SpeedUpItem::Release()
{
}

void SpeedUpItem::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetTag() == "Player");
	{
		DelCollider(*this);
	}
}
