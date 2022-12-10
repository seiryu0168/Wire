#include "EnemyTurret.h"
#include"Engine/Model.h"
#include"Bullet.h"
#include"Engine/SphereCollider.h"
//コンストラクタ
EnemyTurret::EnemyTurret(GameObject* parent)
	:Enemy(parent,"EnemyTurret"),
	hModel_(-1),
	shotTime_(0),
	rpm_(0)

{

}

//デストラクタ
EnemyTurret::~EnemyTurret()
{

}

//初期化
void EnemyTurret::Initialize()
{
	hModel_ = Model::Load("Assets\\Enemy2.fbx");
	assert(hModel_ >= 0);
	
	XMFLOAT3 initPos = transform_.position_;
	initPos.x = rand() % 100;
	initPos.z = rand() % 100;
	RayCastData ray;
	ray.start = { initPos.x, 999.0f, initPos.z };
	ray.dir = { 0,1,0 };
	Model::RayCast(hModel_, ray);
	if (ray.hit)
	{
		initPos.y = 999 - ray.dist;
	}
	transform_.position_ = initPos;
}

//更新
void EnemyTurret::Update()
{

}

void EnemyTurret::Attack()
{
	Shot();
}

void EnemyTurret::Shot()
{
	shotTime_++;
	float isShot = shotTime_ / rpm_;
	if (shotTime_ >= isShot)
	{
		XMVECTOR shotDir = XMVector3Normalize(GetToPlayerVec());
		Bullet* pBullet = Instantiate<Bullet>(GetParent());
		pBullet->SetPosition(this->transform_.position_);
		pBullet->SetDir(shotDir);
		shotTime_ = 0;
	}

}

void EnemyTurret::FixedUpdate()
{

}
//描画
void EnemyTurret::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void EnemyTurret::Release()
{

}