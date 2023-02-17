#include "Test.h"
#include"Engine/DirectX_11/Input.h"
#include"Engine/ResourceManager/Model.h"
#include"Effect.h"
#include"Engine/GameObject/Camera.h"
//コンストラクタ
Test::Test(GameObject* parent)
    :GameObject(parent, "Test"),
    hModel_(-1)
    //pParticle_(nullptr)
{
}

//デストラクタ
Test::~Test()
{
}

//初期化
void Test::Initialize()
{
    transform_.position_ = XMFLOAT3(2.0f, 0.0f, 10.0f);
    
    hModel_=ModelManager::Load("Assets\\TestBox.fbx");
    assert(hModel_ >= 0);
    SphereCollider* pCollider = new SphereCollider(XMFLOAT3(0, 0, 0), 1);
    AddCollider(pCollider);
   /* pParticle_ = Instantiate<Particle>(this);

    EmitterData data;
    data.textureFileName = "Assets\\Effect01.png";
    data.position = XMFLOAT3(0, 3.0f, 0);
    data.positionErr = XMFLOAT3(1.4f, 0, 1.4f);
    data.delay = 0;
    data.number = 1;
    data.lifTime = 100.0f;
    data.acceleration = 0.98f;
    data.gravity = 0.0f;
    data.dir = XMFLOAT3(0, 1, 0);
    data.dirErr = XMFLOAT3(360.0f, 360.0f, 360.0f);
    data.firstSpeed = 0.3f;
    data.speedErr = 0.0f;
    data.size = XMFLOAT2(1.5f,1.5f);
    data.sizeErr = XMFLOAT2(0.3, 0.3);
    data.scale = XMFLOAT2(0.98f, 0.98f);
    data.color = XMFLOAT4(1, 1, 1, 1);
    data.deltaColor = XMFLOAT4(0, 0, 0, -0.02);
    pParticle_->ParticleStart(data);*/
}

//更新
void Test::Update()
{
    //移動
    if (Input::IsKey(DIK_W))
    {
       transform_.position_.z += 0.1f;

    }
    if (Input::IsKey(DIK_S))
    {
        transform_.position_.z += -0.1f;

    }
    if (Input::IsKey(DIK_A))
    {
        transform_.position_.x += -0.1f;

    }
    if (Input::IsKey(DIK_D))
    {
        transform_.position_.x += 0.1f;


    }

    //回転
    if (Input::IsKey(DIK_LEFT))
    {
        transform_.rotate_.y += 1;
    }
    if (Input::IsKey(DIK_RIGHT))
    {
        transform_.rotate_.y += -1;
    }
    if (Input::IsKey(DIK_UP))
    {
        transform_.rotate_.x += 1;

    }
    if (Input::IsKey(DIK_DOWN))
    {
        transform_.rotate_.x += -1;

    }
    if (Input::IsKey(DIK_Q))
    {
        transform_.rotate_.z += 1;
    }
    if (Input::IsKey(DIK_E))
    {
        transform_.rotate_.z += -1;
    }

    //拡縮
    if (Input::IsKey(DIK_Z))
    {
        transform_.scale_.x *= 1.01f;
        transform_.scale_.y *= 1.01f;
        transform_.scale_.z *= 1.01f;
    }
    if (Input::IsKey(DIK_X))
    {
        transform_.scale_.x *= 0.99f;
        transform_.scale_.y *= 0.99f;
        transform_.scale_.z *= 0.99f;
    }

    if (Input::IsKeyDown(DIK_O))
    {
        Camera::SetPosition(XMVectorSet(0, 3, 10,0));
    }

    if (Input::IsKeyDown(DIK_I))
    {
        Camera::SetPosition(XMVectorSet(0, 3, -10, 0));
    }

    RayCastData ray;
    ray.start = XMFLOAT3(0, 0, 0);
    ray.dir = XMFLOAT3(1, 0, 0);

    ModelManager::RayCast(hModel_,ray);
    if (ray.hit)
    {
        bool a = ray.hit;
        XMFLOAT3 b;
        XMFLOAT3 c;
        XMStoreFloat3(&b, ray.hitPos);
        XMStoreFloat3(&c, ray.normal);
        int g = 10;
    }

}
void Test::FixedUpdate()
{

}
//描画
void Test::Draw()
{
    ModelManager::SetTransform(hModel_, transform_);
    ModelManager::Draw(hModel_);
}

//開放
void Test::Release()
{
}

void Test::OnCollision(GameObject* pTarget)
{
    //KillMe();
}