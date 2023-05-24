#include "SearchUpItem.h"

#include "SearchUpItem.h"
#include"Engine/Collider/BoxCollider.h"
namespace
{
	static const float SEARCH = XMConvertToRadians(30.0f);
	static const int LIFE = 600;
	static const int PARTICLE_NUM = 1;
	static const float EFFECT_SPEED = 0.03f;
	static const XMFLOAT3 posErr = { 3.0f,0.0f,3.0f };
	static const int DELAY = 0;
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
	pData_ = std::make_unique<EmitterData>();
	PlayParticle(transform_.position_);
}

void SearchUpItem::Update()
{
	if (IsAttach())
	{
		SubtractTime();
		pData_->position = GetAttacheObject()->GetPosition();
		pParticle_->ParticleStart(*pData_.get());
	}
}

void SearchUpItem::Draw()
{
	if (IsCollision()==false)
	{
		ModelManager::SetTransform(hModel_, transform_);
		ModelManager::Draw(hModel_);
	}
}

void SearchUpItem::Release()
{
}

void SearchUpItem::PlayParticle(XMFLOAT3 pos)
{
	pData_->textureFileName = "Assets\\Smoke.png";
	pData_->position = pos;
	pData_->positionErr = XMFLOAT3(1.0f, 0, 1.0f);
	pData_->delay = 10;
	pData_->number = 2;
	pData_->lifTime = 120;
	pData_->acceleration = 0.98f;
	pData_->gravity = 0.0f;
	pData_->dir = { 0.0f,1.0f,0.0f };
	pData_->firstSpeed = 0.1f;
	pData_->speedErr = 0.2f;
	pData_->size = XMFLOAT2(1.5f, 1.5f);
	pData_->sizeErr = XMFLOAT2(0.3f, 0.3f);
	pData_->scale = XMFLOAT2(0.98f, 0.98f);
	pData_->color = XMFLOAT4(0, 0.5f, 0.5f, 1);
	pData_->deltaColor = XMFLOAT4(0, 0, 0, -0.01f);
	pParticle_->ParticleStart(*pData_);
}
void SearchUpItem::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetTag() == "Player")
	{
		if (pTarget->GetTag() == "Player")
		{
			pData_->firstSpeed = EFFECT_SPEED;
			pData_->positionErr = posErr;
			pData_->number = PARTICLE_NUM;
			pData_->delay = DELAY;
			pParticle_->KillEmitter(hEmitter_);
			SetCollision();
			DelCollider(*this);
		}
	}
}
