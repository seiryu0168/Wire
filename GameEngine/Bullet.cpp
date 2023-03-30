#include "Bullet.h"
#include"Engine/DirectX_11/Particle.h"
#include"Engine/ResourceManager/Audio.h"
#include"Engine/Collider/SphereCollider.h"
#include"Engine/ResourceManager/Model.h"
#include"Engine/GameObject/Camera.h"
//コンストラクタ
Bullet::Bullet(GameObject* parent)
	:GameObject(parent,"Bullet"),
	hModel_(-1),
	hAudio_(-1),
	life_(600),
	speed_(0.8f),
	dir_(XMVectorSet(0,0,1,0)),
	pBill_(nullptr),
	pParticle_(nullptr)
{

}

//デストラクタ
Bullet::~Bullet()
{

}

//初期化
void Bullet::Initialize()
{
	SetTag("EnemyBullet");
	SphereCollider* pCollision = new SphereCollider(XMFLOAT3(0, 0, 0), 0.3f);
	AddCollider(pCollision);
	pBill_ = new BillBoard();
	if (pParent_ != nullptr)
	{
		transform_.position_ = pParent_->GetPosition();
	}
	else
		KillMe();
	hAudio_ = Audio::Load("Assets\\explosion.wav",10);
	assert(hAudio_ >= 0);

	pParent_ = GetParent()->GetParent();
	pBill_->Load("Assets\\Effect01.png");
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

//描画
void Bullet::Draw()
{
	XMMATRIX matW = transform_.GetLocalScaleMatrix() * Camera::GetBillBoardMatrix() * transform_.GetWorldTranslateMatrix();
	
	pBill_->Draw(matW, { 1,1,1,1 });
}

void Bullet::OnCollision(GameObject* target)
{
	if (target->GetObjectName() == "Player")
	{
		Audio::Play(hAudio_);
		pParticle_ = Instantiate<Particle>(GetParent());

		EmitterData data;

		data.textureFileName = "Assets\\Smoke.png";
		data.position = transform_.position_;
		data.positionErr = XMFLOAT3(0.2f, 0.0f, 0.2f);
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
		KillMe();
	}
}

void Bullet::Release()
{
	pBill_->Release();
}