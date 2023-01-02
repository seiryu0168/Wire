#include "EnemyTurret.h"
#include"Engine/Model.h"
#include"Bullet.h"
#include"HomingBullet.h"
#include"Engine/SphereCollider.h"
//コンストラクタ
EnemyTurret::EnemyTurret(GameObject* parent)
	:Enemy(parent,"EnemyTurret"),
	hModel_(-1),
	shotTime_(0),
	rpm_(300)

{

}

//デストラクタ
EnemyTurret::~EnemyTurret()
{

}

//初期化
void EnemyTurret::Initialize()
{
	hModel_ = ModelManager::Load("Assets\\Enemy2.fbx");
	assert(hModel_ >= 0);
	
	XMFLOAT3 initPos = transform_.position_;
	initPos.x = rand() % 100;
	initPos.z = rand() % 100;
	RayCastData ray;
	ray.start = { initPos.x, 999.0f, initPos.z };
	ray.dir = { 0,1,0 };
	ModelManager::RayCast(hModel_, ray);
	if (ray.hit)
	{
		initPos.y = 999 - ray.dist;
	}
	transform_.position_ = initPos;
	SetPlayerPointer((Player*)FindObject("Player"));
}

//更新
void EnemyTurret::Update()
{
	SetPositionVec(XMLoadFloat3(&transform_.position_));
	GetEnemyState()->Update(this);
}

void EnemyTurret::Attack()
{
	Shot();
}

void EnemyTurret::Shot()
{
	shotTime_++;
	float isShot = 3600.0f / rpm_;
	if (isShot<=(float)shotTime_)
	{
		XMVECTOR shotDir = XMVector3Normalize(GetToPlayerVec());
		HomingBullet* pHBullet = Instantiate<HomingBullet>(this);
		pHBullet->SetDir(shotDir);
		shotTime_ = 0;
	}

}

void EnemyTurret::FixedUpdate()
{

}
//描画
void EnemyTurret::Draw()
{
	ModelManager::SetTransform(hModel_, transform_);
	ModelManager::Draw(hModel_);
}

void EnemyTurret::Release()
{

}