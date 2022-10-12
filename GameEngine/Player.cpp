#include "Player.h"
#include"Engine/Input.h"
#include"Engine/Model.h"
#include"Engine/Camera.h"
#include"Engine/SceneManager.h"
#include"EngineTime.h"
//コンストラクタ
Player::Player(GameObject* parent)
    :GameObject(parent, "Player"),hModel_(-1),vCamPos(XMVectorSet(0,3,10,0)),matCam(XMMatrixIdentity()),angleY(0),angleX(0)

{
}

//デストラクタ
Player::~Player()
{
}

//初期化
void Player::Initialize()
{
    //transform_.position_ = XMFLOAT3(-2.0f, 0.0f, 0.0f);
    hModel_ = Model::Load("Assets\\TestBox.fbx");
    assert(hModel_ >= 0);
    BoxCollider* pCollider = new BoxCollider(XMFLOAT3(0, 0, 0), XMFLOAT3(1,4,1));
    AddCollider(pCollider);
}

//更新
void Player::Update()
{
    float sd = 0;
    
    CameraMove();
    if (Input::IsPadButton(XINPUT_GAMEPAD_A))
    {
        int a = 10;
    }
}

void Player::FixedUpdate()
{
   
}

//描画
void Player::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

//開放
void Player::Release()
{
}

void Player::CameraMove()
{
    if (Input::IsKey(DIK_A))
    {
        angleY++;

    }
    if (Input::IsKey(DIK_D))
    {
        angleY--;
    }
    if (Input::IsKey(DIK_W))
    {
        angleX--;

    }
    if (Input::IsKey(DIK_S))
    {
        angleX++;
    }


    vPlayerPos = XMLoadFloat3(&transform_.position_);
    XMVECTOR vMoveCam;
    matCam = XMMatrixRotationX(angleX * (M_PI / 180));
    matCam *= XMMatrixRotationY(angleY * (M_PI / 180));
    vMoveCam = XMVector3TransformCoord(vCamPos, matCam);
    Camera::SetPosition(vPlayerPos + vMoveCam);
}

void Player::OnCollision(GameObject* pTarget)
{
    //KillMe();
}