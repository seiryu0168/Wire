#include "HomingBullet.h"
#include"Player.h"
#include"Engine/Camera.h"
#include"Engine/Particle.h"
HomingBullet::HomingBullet(GameObject* parent)
	:GameObject(parent, "HomingBullet"),
	hModel_(-1),
	life_(320),
	period_(180),
	position_(XMVectorSet(0, 0, 0, 0)),
	velocity_(XMVectorSet(0, 0, 0, 0)),
	pBill_(nullptr),
	pParticle_(nullptr)
{
}

HomingBullet::~HomingBullet()
{
}

void HomingBullet::Initialize()
{
	pBill_ = new BillBoard;
	transform_.position_ = pParent_->GetPosition();
	pBill_->Load("Assets\\Effect01.png");
	pParent_ = FindObject("TitleScene");
	pPlayer_ = (Player*)FindObject("Player");
	position_ = XMLoadFloat3(&transform_.position_);
	velocity_ = XMVectorSet(0, 2, 0, 0);
}

void HomingBullet::Update()
{
	
	Homing();
	if (life_ < 0)
	{
		KillMe();
	}life_--;
}

void HomingBullet::FixedUpdate()
{
}

void HomingBullet::Homing()
{
	if (period_ < 0)
	{
		return;
	}
	XMVECTOR acceleration = XMVectorSet(0, 0, 0, 0);
	XMFLOAT3 targetPos = pPlayer_->GetPosition();
	XMVECTOR diff = XMLoadFloat3(&targetPos) - position_;
	acceleration += ((diff - velocity_ * (period_ / 60.0f)) * 2.0f) / (((float)period_ / 60.0f) * ((float)period_ / 60.0f));
	period_--;
	if (period_ > 60)
	{
		
		float a = XMVectorGetX(XMVector3Normalize(acceleration));
		if (std::abs(a) > 0.8f)
		{
			acceleration = XMVector3Normalize(acceleration) * 0.8f;
		}
		velocity_ += acceleration * 0.08f;
	}
	position_ += velocity_ * 0.08f;
	XMStoreFloat3(&transform_.position_, position_);
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
		pParticle_ = Instantiate<Particle>(GetParent());
		EmitterData data;
		
		XMVECTOR pos = XMLoadFloat3(&transform_.position_);
		data.textureFileName = "Assets\\Effect01.png";
		data.position = transform_.position_;
		data.positionErr = XMFLOAT3(0.2, 0, 0.2);
		data.delay = 0;
		data.number = 30;
		data.lifTime = 50.0f;
		data.acceleration = 0.98f;
		data.gravity = 0.0f;

		
		data.dir = {0,0,0};
		data.dirErr = XMFLOAT3(360.0f, 360.0f, 360.0f);
		data.firstSpeed = 0.9f;
		data.speedErr = 0.2f;
		data.size = XMFLOAT2(1.5f, 1.5f);
		data.sizeErr = XMFLOAT2(0.3f, 0.3f);
		data.scale = XMFLOAT2(0.98f, 0.98f);
		data.color = XMFLOAT4(1, 1, 1, 1);
		data.deltaColor = XMFLOAT4(0, 0, 0, -0.08);
		pParticle_->ParticleStart(data);
		KillMe();
	}
}

void HomingBullet::Release()
{
}
