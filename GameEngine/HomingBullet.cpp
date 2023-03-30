#include "HomingBullet.h"
#include"Player.h"
#include"Engine/ResourceManager/Audio.h"
#include"Engine/GameObject/Camera.h"
#include"Engine/DirectX_11/Particle.h"
#include"Engine/Collider/SphereCollider.h"
namespace
{
	static const float frame = 60.0f;	
}
HomingBullet::HomingBullet(GameObject* parent)
	:GameObject(parent, "HomingBullet"),
	hModel_(-1),
	hAudio_(-1),
	life_(320),
	period_(180),
	position_(XMVectorSet(0, 0, 0, 0)),
	velocity_(XMVectorSet(0, 0, 0, 0)),
	pPlayer_(nullptr),
	pBill_(nullptr),
	pParticle_(nullptr)
{
}

HomingBullet::~HomingBullet()
{
}

void HomingBullet::Initialize()
{
	SphereCollider* pCollision = new SphereCollider(XMFLOAT3(0, 0, 0),0.3f);
	AddCollider(pCollision);

	hAudio_ = Audio::Load("Assets\\explosion.wav",10);
	assert(hAudio_ >= 0);
	SetTag("EnemyBullet");
	pBill_ = new BillBoard;
	transform_.position_ = pParent_->GetPosition();
	pBill_->Load("Assets\\Effect01.png");
	pParent_ = FindObject("PlayScene");
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
		KillMe();
	}

	XMVECTOR acceleration = XMVectorSet(0, 0, 0, 0);
	XMFLOAT3 targetPos = pPlayer_->GetPosition();
	XMVECTOR diff = XMLoadFloat3(&targetPos) - position_;
	acceleration += ((diff - velocity_ * (period_ / frame)) * 2.0f) / (((float)period_ / frame) * ((float)period_ / frame));
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

void HomingBullet::BeforeDeath()
{
	
	pParticle_ = Instantiate<Particle>(GetParent());

	EmitterData data;

	data.textureFileName = "Assets\\Smoke.png";
	data.position = transform_.position_;
	data.positionErr = XMFLOAT3(0.2f, 0, 0.2f);
	data.delay = 0;
	data.number = 30;
	data.lifTime = 600;
	data.acceleration = 0.98f;
	data.gravity = 0.0f;


	data.dir = { 0,0,0 };
	data.dirErr = XMFLOAT3(360.0f, 360.0f, 360.0f);
	data.firstSpeed = 0.8f;
	data.speedErr = 0.2f;
	data.size = XMFLOAT2(0.8f, 0.8f);
	data.sizeErr = XMFLOAT2(0.4f, 0.4f);
	data.scale = XMFLOAT2(1.1f, 1.1f);
	data.color = XMFLOAT4(1, 1, 0.1f, 1);
	data.deltaColor = XMFLOAT4(0, (float)(-1.0f / 20.0f), 0, (float)(-1.0f / 20.0f));
	pParticle_->ParticleStart(data);


	data.position = transform_.position_;
	data.positionErr = XMFLOAT3(0.5f, 0.5f, 0.5f);
	data.delay = 0;
	data.number = 10;
	data.lifTime = 50;
	data.acceleration = 0.98f;
	data.gravity = 0.0f;


	data.dir = { 0,0,0 };
	data.dirErr = XMFLOAT3(360.0f, 360.0f, 360.0f);
	data.firstSpeed = 0.6f;
	data.speedErr = 0.2f;
	data.size = XMFLOAT2(0.1f, 0.1f);
	data.sizeErr = XMFLOAT2(0, 0);
	data.scale = XMFLOAT2(0.99f, 0.99f);
	data.color = XMFLOAT4(1, 1, 0.1f, 1);
	data.deltaColor = XMFLOAT4(0, 0, 0, 0);
	pParticle_->ParticleStart(data);
}

void HomingBullet::OnCollision(GameObject* target)
{
	if (target->GetObjectName() == "Player")
	{
		Audio::Play(hAudio_);
		KillMe();
	}
}

void HomingBullet::Release()
{
}
