#include "SearchUpItem.h"

#include "SearchUpItem.h"
#include"Engine/Collider/BoxCollider.h"
namespace
{
	static const float SEARCH = XMConvertToRadians(30.0f);
	static const int LIFE = 600;
}
SearchUpItem::SearchUpItem(GameObject* parent)
	:ItemBase(parent, "SearchUpItem"),
	hModel_(-1)
{
	value_ = SEARCH;
}

SearchUpItem::~SearchUpItem()
{
}

void SearchUpItem::Initialize()
{
	SetLifeTime(LIFE);
	BoxCollider* pCollision = new BoxCollider({ 0,0,0 }, { 1,1,1 });
	AddCollider(pCollision);
	hModel_ = ModelManager::Load("Assets\\Mark.fbx");
	SetItemType(ITEM_TYPE::SEARCH);
}

void SearchUpItem::Update()
{
	if (IsAttach())
	{
		SubtractTime();
	}
}

void SearchUpItem::Draw()
{
	ModelManager::SetTransform(hModel_, transform_);
	ModelManager::Draw(hModel_);
}

void SearchUpItem::Release()
{
}

void SearchUpItem::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetTag() == "Player")
	{
		DelCollider(*this);
	}
}
