#include "HomingBullet.h"
#include"Player.h"
#include"Engine/Camera.h"
HomingBullet::HomingBullet(GameObject* parent)
	:GameObject(parent,"HomingBullet"),
	hModel_(-1),
	life_(10),
	position_(XMVectorSet(0, 0, 0, 0)),
	velocity_(XMVectorSet(0, 0, 0, 0))
{
}

HomingBullet::~HomingBullet()
{
}

void HomingBullet::Initialize()
{
	pBill_ = new BillBoard;
	pBill_->Load("Assets\\Effect01.png");
	pParent_ = FindObject("TitleScene");
	pPlayer_ = (Player*)FindObject("Player");

}

void HomingBullet::Update()
{
	XMVECTOR acceleration = XMVectorSet(0,0,0,0);
	XMFLOAT3 targetPos = pPlayer_->GetPosition();
	XMVECTOR diff = XMLoadFloat3(&targetPos) - position_;
	acceleration += (((diff - velocity_) * 1.0f) * 2.0f )/ (1.0f * 1.0f);

	velocity_ += acceleration*0.1f;
	position_ += velocity_*0.1f;
	XMStoreFloat3(&transform_.position_, position_);

	if (life_ < 0)
	{
		KillMe();
	}
}

void HomingBullet::FixedUpdate()
{
}

void HomingBullet::Draw()
{
	XMMATRIX matW = transform_.GetLocalScaleMatrix() * Camera::GetBillBoardMatrix() * transform_.GetWorldTranslateMatrix();

	pBill_->Draw(matW, { 1,1,1,1 });
}

void HomingBullet::OnCollision(GameObject* target)
{
	if (target->GetObjectName() == "Player")
	{
		KillMe();
	}
}

void HomingBullet::Release()
{
}
