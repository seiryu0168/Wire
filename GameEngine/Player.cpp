#include "Player.h"
#include"Engine/Input.h"
#include"Engine/Model.h"
#include"Engine/Camera.h"
#include"Engine/SceneManager.h"
#include"EngineTime.h"
//コンストラクタ
Player::Player(GameObject* parent)
    :GameObject(parent, "Player"),
    hModel_(-1),
    vCamPos(XMVectorSet(0, 3, -10, 0)),
    vPlayerPos(XMVectorSet(0,0,0,0)),
    matCam(XMMatrixIdentity()),
    speed(4.0f),
    angleY(0),
    angleX(0)

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
    CameraMove();
    transform_.position_.x += Input::GetLStick_X();
    transform_.position_.z += Input::GetLStick_Y();


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
    angleX += Input::GetRStick_X();
    angleY += Input::GetRStick_Y();

    vPlayerPos = XMLoadFloat3(&transform_.position_);
    Camera::SetTarget(vPlayerPos);
    XMVECTOR vMoveCam;
    matCam = XMMatrixRotationX(angleY*speed * (M_PI / 180));
    matCam *= XMMatrixRotationY(angleX*speed * (M_PI / 180));
    vMoveCam = XMVector3TransformCoord(vCamPos, matCam);
    
    Camera::SetPosition(vPlayerPos + vMoveCam);
}

void Player::OnCollision(GameObject* pTarget)
{
    //KillMe();
}