#include "Test.h"
#include"Engine/Input.h"
#include"Engine/Model.h"
#include"Effect.h"
#include"Engine/Camera.h"
//コンストラクタ
Test::Test(GameObject* parent)
    :GameObject(parent, "Test"),hModel_(-1)
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
    
    hModel_=Model::Load("Assets\\TestBox.fbx");
    //hModel_=Model::Load("Assets\\BlueBall.fbx");
    //hModel_=Model::Load("Assets\\BlueBall.fbx");
    assert(hModel_ >= 0);
    SphereCollider* pCollider = new SphereCollider(XMFLOAT3(0, 0, 0), 1);
    AddCollider(pCollider);
}

//更新
void Test::Update()
{
    //移動
    if (Input::IsKey(DIK_W))
    {
       transform_.position_.z += 0.1;

    }
    if (Input::IsKey(DIK_S))
    {
        transform_.position_.z += -0.1;

    }
    if (Input::IsKey(DIK_A))
    {
        transform_.position_.x += -0.1;

    }
    if (Input::IsKey(DIK_D))
    {
        transform_.position_.x += 0.1;

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

    Model::RayCast(hModel_,ray);
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
    Model::SetTransform(hModel_, transform_);
    //Model::Draw(hModel_);
    aaa.Draw(transform_.GetWorldMatrix(), XMFLOAT4(0, 1, 0, 1));

}

//開放
void Test::Release()
{
}

void Test::OnCollision(GameObject* pTarget)
{
    //KillMe();
}