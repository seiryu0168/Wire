#include "PlayerBase.h"
#include"Engine/ResourceManager/Model.h"
#include"Engine/ResourceManager/Audio.h"
#include"Engine/DirectX_11/Particle.h"
#include"Player.h"
PlayerBase::PlayerBase(GameObject* parent)
    :GameObject(parent, "PlayerBase"),
    hModel_(-1),
    prevPos_(XMFLOAT3(0, 0, 0))
{
	pParticle_ = Instantiate<Particle>(this);
}

PlayerBase::~PlayerBase()
{
}

void PlayerBase::Initialize()
{
	hModel_ = ModelManager::Load("Assets\\Player_Maya.fbx");
	assert(hModel_ >= 0);
    hAudio_ = Audio::Load("Assets\\move.wav");
    assert(hAudio_ >= 0);
    vSparkPos_[static_cast<int>(SPARKPOS::FRONT_LEFT)] = XMVectorSet(-3.0f, -1.0f, 3.0f, 0);
    vSparkPos_[static_cast<int>(SPARKPOS::FRONT_RIGHT)] = XMVectorSet(3.0f, -1.0f, 3.0f, 0);
    vSparkPos_[static_cast<int>(SPARKPOS::BACK_LEFT)] = XMVectorSet(-3.0f, -1.0f, -3.0f, 0);
    vSparkPos_[static_cast<int>(SPARKPOS::BACK_RIGHT)] = XMVectorSet(3.0f, -1.0f, -3.0f, 0);

}

void PlayerBase::Update()
{
	//ˆÚ“®‚ÌŒü‚«
	moveVec_ = (GetParent()->GetPosition() - prevPos_);
	//¡‚ÌÀ•W‚ð‹L˜^
	prevPos_ = GetParent()->GetPosition();
    float l = VectorLength(moveVec_);
    bool f = ((Player*)GetParent())->IsFly();
    bool j = ((Player*)GetParent())->IsJump();
    bool a = ((Player*)GetParent())->IsAir();
    if (((Player*)GetParent())->IsGround() && VectorLength(moveVec_) >= 0.2f)
    {
    Spark();
    Audio::Play(hAudio_);
    }
}

void PlayerBase::Draw()
{
	ModelManager::SetTransform(hModel_, transform_);
	ModelManager::Draw(hModel_);

}

void PlayerBase::Release()
{
}

void PlayerBase::Spark()
{
	EmitterData data;
    for (int i = 0; i < static_cast<int>(SPARKPOS::MAX); i++)
    {
        XMVECTOR VPos = vSparkPos_[i] * GetParent()->GetTransform().GetWorldRotateMatrix();
        XMFLOAT3 playerPos = GetParent()->GetPosition();
        data.textureFileName = "Assets\\Effect01.png";
        data.position = StoreFloat3(XMLoadFloat3(&playerPos) + VPos);
        data.positionErr = XMFLOAT3(0.2f, 0, 0.2f);
        data.delay = 0;
        data.number = 1;
        data.lifTime = 30;
        data.acceleration = 0.58f;
        data.gravity = 0.2f;

        data.dir = StoreFloat3((-moveVec_)+ XMVectorSet(0, VectorLength(moveVec_)*0.6f, 0, 0));
        data.dirErr = XMFLOAT3(10.0f, 5.0f, 10.0f);
        data.firstSpeed = VectorLength(moveVec_)*0.5f;
        data.speedErr = 1.0f;
        data.size = XMFLOAT2(0.3f, 0.3f);
        data.sizeErr = XMFLOAT2(0.1f, 0.1f);
        data.scale = XMFLOAT2(0.9f, 0.9f);
        data.color = XMFLOAT4(1, 1, 0, 1);
        data.deltaColor = XMFLOAT4(0, 0, 0, -0.08f);
        pParticle_->ParticleStart(data);
    }


}
