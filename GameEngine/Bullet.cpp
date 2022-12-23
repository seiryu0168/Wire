#include "Bullet.h"
#include"Engine/Model.h"
#include"Engine/Camera.h"
//コンストラクタ
Bullet::Bullet(GameObject* parent)
	:GameObject(parent,"Bullet"),
	hModel_(-1),
	life_(600),
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
	pBill_ = new BillBoard();
	pBill_->Load("Assets\\Effect01.png");
	//hModel_ = ModelManager::Load("Assets\\Mark.fbx");
	//assert(hModel_ >= 0);
}

//更新
void Bullet::Update()
{
	XMVECTOR vPosition = XMLoadFloat3(&transform_.position_);
	vPosition += dir_ * speed_;
	XMStoreFloat3(&transform_.position_, vPosition);
	if (life_ <= 0)
	{
		KillMe();
	}
	life_--;
}

void Bullet::FixedUpdate()
{

}
//描画
void Bullet::Draw()
{
	XMMATRIX matW =XMMatrixScaling(transform_.scale_.x, transform_.scale_.y, transform_.scale_.z)*Camera::GetBillBoardMatrix()* XMMatrixTranslation(transform_.position_.x, transform_.position_.y, transform_.position_.z);
	
	pBill_->Draw(matW, { 1,1,1,1 });
	//ModelManager::SetTransform(hModel_, transform_);
	//ModelManager::Draw(hModel_);
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
	pBill_->Release();
}