#include "Player.h"
#include"Engine/Input.h"
#include"Engine/Model.h"
#include"Engine/Camera.h"
#include"Stage1.h"
#include"Engine/SceneManager.h"
#include"EngineTime.h"
//コンストラクタ
Player::Player(GameObject* parent)
    :GameObject(parent, "Player"),
    hModel_(-1),
    vCamPos(XMVectorSet(0, 3, -10, 0)),
    vPlayerPos(XMVectorSet(0,0,0,0)),
    vPlayerMove(XMVectorSet(0,0,0,0)),
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
    stageNum_ = ((Stage1*)GetParent()->FindChild("Stage1"))->GetModelHandle();
}

//更新
void Player::Update()
{
    CameraMove();
 
    XMVECTOR vMove;
    vMove = XMVectorSet(Input::GetLStick_X(), 0, Input::GetLStick_Y(), 0);
    vMove = XMVector3TransformCoord(vMove, matCam);
    XMFLOAT3 Move;
    XMStoreFloat3(&Move, vMove);
    transform_.position_.x += Move.x;
    transform_.position_.z += Move.z;


    if (Input::GetLTrigger())
    {

        float w = (float)Direct3D::screenWidth / 2.0f;
        float h = (float)Direct3D::screenHeight / 2.0f;

        XMMATRIX vp = { w, 0, 0, 0,
                        0,-h, 0, 0,
                        0, 0, 1, 0,
                        w, h, 0, 1
                      };

        XMMATRIX invVp = XMMatrixInverse(nullptr, vp);
        XMMATRIX invVw = XMMatrixInverse(nullptr, Camera::GetViewMatrix());
        XMMATRIX invPr = XMMatrixInverse(nullptr, Camera::GetProjectionMatrix());

        XMFLOAT3 ptrFront = { w, h, 0 };
        XMVECTOR vPtrFront = XMLoadFloat3(&ptrFront);
        XMFLOAT3 ptrBack = { w, h, 1 };
        XMVECTOR vPtrBack = XMLoadFloat3(&ptrBack);
 
        vPtrFront = XMVector3Normalize(XMVector3TransformCoord(vPtrFront, invVp * invPr * invVw));
        vPtrBack = XMVector3Normalize(XMVector3TransformCoord(vPtrBack, invVp * invPr * invVw));

        RayCastData ray;
        XMStoreFloat3(&ray.start, vPtrFront);
        XMStoreFloat3(&ray.dir, vPtrBack - vPtrFront);
        Model::RayCast(stageNum_, ray);

        if (ray.hit)
        {
            XMFLOAT3 hitPosition;
            XMStoreFloat3(&hitPosition, ray.hitPos);
        }
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