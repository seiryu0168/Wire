#include "Bullet.h"
#include"Engine/Model.h"
//コンストラクタ
Bullet::Bullet(GameObject* parent)
	:GameObject(parent,"Bullet"),
	hModel_(-1),
	speed_(0.8f),
	dir_(XMVectorSet(0,0,1,0))
{

}

//デストラクタ
Bullet::~Bullet()
{

}

//初期化
void Bullet::Initialize()
{
	if (life_ <= 0)
	{
		KillMe();
	}
	hModel_ = Model::Load("Assets\\Mark.fbx");
	assert(hModel_ >= 0);
	life_--;
}

//更新
void Bullet::Update()
{
	XMVECTOR vPosition = XMLoadFloat3(&transform_.position_);
	vPosition += dir_ * speed_;
}

void Bullet::FixedUpdate()
{

}
//描画
void Bullet::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Bullet::OnCollision(GameObject* target)
{
	if (target->GetObjectName() == "Player")
	{
		KillMe();
	}
}

void Bullet::Release()
{

}