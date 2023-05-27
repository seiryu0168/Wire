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

	static const std::string ITEM_MODEL = "Assets\\Model\\SearchUpItem.fbx";
	static const std::string PARTICLE_IMAGE = "Assets\\Image\\Smoke.png";
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
	//効果時間やらアイテムタイプの設定など
	SetLifeTime(LIFE);
	BoxCollider* pCollision = new BoxCollider({ 0,0,0 }, { 2,2,2 });
	AddCollider(pCollision);
	hModel_ = ModelManager::Load(ITEM_MODEL);
	SetItemType(ITEM_TYPE::SEARCH);
	pData_ = std::make_unique<EmitterData>();
	PlayParticle(transform_.position_);
}

void SearchUpItem::Update()
{
	//プレイヤーに効果が適用されていたら
	if (IsAttach())
	{
		SubtractTime();
		pData_->position = GetAttacheObject()->GetPosition();
		pParticle_->ParticleStart(*pData_.get());
	}
}

void SearchUpItem::Draw()
{
	//プレイヤーと一度も当たってなかったら
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
	pData_->textureFileName = PARTICLE_IMAGE;
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
			//エフェクトの変数を変える
			pData_->firstSpeed = EFFECT_SPEED;
			pData_->positionErr = posErr;
			pData_->number = PARTICLE_NUM;
			pData_->delay = DELAY;
			pParticle_->KillEmitter(hEmitter_);

			//当たったフラグを立てる(いい名前思いつかなかった)
			SetCollision();
			//当たり判定を消す
			DelCollider(*this);
		}
}
