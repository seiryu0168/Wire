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
    vCamPos(XMVectorSet(0, 5, -15, 0)),
    vPlayerPos(XMVectorSet(0,0,0,0)),
    vPlayerMove(XMVectorSet(0,0,0,0)),
    vBaseTarget(XMVectorSet(0,0,5,0)),
    matCam(XMMatrixIdentity()),
    speed(4.0f),
    angleY(0),
    angleX(0),
    flyFlag(false)

{
}

//デストラクタ
Player::~Player()
{
}

//初期化
void Player::Initialize()
{
    hModel_ = Model::Load("Assets\\TestBox.fbx");
    assert(hModel_ >= 0);
    BoxCollider* pCollider = new BoxCollider(XMFLOAT3(0, 0, 0), XMFLOAT3(1, 4, 1));
    AddCollider(pCollider);
    stageNum_ = ((Stage1*)GetParent()->FindChild("Stage1"))->GetModelHandle();
    transform_.position_ = XMFLOAT3(-24, 20, 10);
    /*transform_.position_.x = -24;
    transform_.position_.z = 10;
    XMFLOAT3 startPos = transform_.position_;
    startPos.y += 10;
    RayCastData firstRay;
    firstRay.start = startPos;*/
    
    rayDir[0] = XMVectorSet(0, 1, 0, 0);
    rayDir[1] = XMVectorSet(0, -1, 0,0);
    rayDir[2] = XMVectorSet(1, 0, 0,0);
    rayDir[3] = XMVectorSet(-1, 0, 0,0);
    rayDir[4] = XMVectorSet(0, 0, 1,0);
    rayDir[5] = XMVectorSet(0, 0, -1,0);
    
   /* firstRay.dir = rayDir[1];
    Model::RayCast(stageNum_, firstRay);

    if (firstRay.hit)
    {
        transform_.position_.y = firstRay.dist - transform_.position_.y - 2;
    }*/
}

//更新
void Player::Update()
{
    XMVECTOR vMove;
    vMove = XMVectorSet(Input::GetLStick_X(), 0, Input::GetLStick_Y(), 0);
    vMove = XMVector3TransformCoord(vMove, matCam);
    XMFLOAT3 Move;
    XMStoreFloat3(&Move, vMove);
    transform_.position_.x += Move.x;
    transform_.position_.z += Move.z;
   // transform_.position_.y -= 0.3;

        vPlayerPos = XMLoadFloat3(&transform_.position_);
    if (Input::GetLTrigger())
    {

        float w = (float)Direct3D::screenWidth / 2.0f;
        float h = (float)Direct3D::screenHeight / 2.0f;
        //ビューポート行列作成
        XMMATRIX vp = { w, 0, 0, 0,
                        0,-h, 0, 0,
                        0, 0, 1, 0,
                        w, h, 0, 1
                      };

        //ビューポート(vp),ビュー,プロジェクションの逆行列を作る
        XMMATRIX invVp = XMMatrixInverse(nullptr, vp);
        XMMATRIX invVw = XMMatrixInverse(nullptr, Camera::GetViewMatrix());
        XMMATRIX invPr = XMMatrixInverse(nullptr, Camera::GetProjectionMatrix());
     
        //画面の中央の
        //XMFLOAT3 ptrFront = { w, h, 0 };
        //XMFLOAT3 ptrBack = { w, h, 1 };
        XMVECTOR vPtrBack = XMVector3TransformCoord(vBaseTarget, matCam);//XMLoadFloat3(&ptrBack);
 
        RayCastData ray;
        XMStoreFloat3(&ray.start, vPlayerPos);
        XMStoreFloat3(&ray.dir, vPtrBack);
        Model::RayCast(stageNum_, ray);
            
        if (Input::IsPadButtonDown(XINPUT_GAMEPAD_A))
        {
            XMFLOAT3 nmlCheck;
            XMStoreFloat3(&nmlCheck,XMVector3Dot(-(XMLoadFloat3(&ray.dir)), ray.normal));
            float nmlX = acos(nmlCheck.x);
            float nmlY= acos(nmlCheck.y);
            float nmlZ= acos(nmlCheck.z);
            if (ray.hit && abs(nmlX) <= (M_PI/2) && abs(nmlY) <= (M_PI/2) && abs(nmlZ) <= (M_PI/2))
            {
                vFlyMove = XMVector3Normalize(ray.hitPos - vPlayerPos);
                //XMStoreFloat3(&transform_.position_, ray.hitPos);
                flyFlag = true;
            }
        }
    }

    if (flyFlag)
    {
        XMStoreFloat3(&transform_.position_, vPlayerPos + vFlyMove);

    }

    CharactorControll();
    CameraMove();

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
    angleX += Input::GetRStick_Y() * speed;
    angleY += Input::GetRStick_X() * speed;

    //vPlayerPos = XMLoadFloat3(&transform_.position_);
    XMVECTOR vMoveCam;
    XMVECTOR vTarCam;
    if (angleX <= -90)
    {
        angleX = -89;
    }
    if (angleX >= 70)
    {
        angleX = 69;
    }
    matCam = XMMatrixRotationX(angleX * (M_PI / 180));
    matCam *= XMMatrixRotationY(angleY * (M_PI / 180));
    vMoveCam = XMVector3TransformCoord(vCamPos, matCam);
    vTarCam = XMVector3TransformCoord(vBaseTarget, matCam);
    
    Camera::SetTarget(vPlayerPos+vTarCam);
    Camera::SetPosition(vPlayerPos + vMoveCam);
}

void Player::CharactorControll()
{
    RayCastData FRay;
    RayCastData BRay;
    RayCastData LRay;
    RayCastData RRay;
    FRay.start = transform_.position_;
    BRay.start = transform_.position_;
    LRay.start = transform_.position_;
    RRay.start = transform_.position_;
    XMStoreFloat3(&FRay.dir, rayDir[4]);
    XMStoreFloat3(&BRay.dir, rayDir[5]);
    XMStoreFloat3(&LRay.dir, rayDir[3]);
    XMStoreFloat3(&RRay.dir, rayDir[2]);

    Model::RayCast(stageNum_, FRay);
    Model::RayCast(stageNum_, BRay);
    Model::RayCast(stageNum_, LRay);
    Model::RayCast(stageNum_, RRay);

    if (FRay.dist <= 1)
    {
        transform_.position_.z -= FRay.dist;
        flyFlag = false;
    }

    if (BRay.dist <= 1)
    {
        transform_.position_.z += BRay.dist;
        flyFlag = false;
    }

    if (LRay.dist <= 1)
    {
        transform_.position_.x += LRay.dist;
        flyFlag = false;
    }

    if (RRay.dist <= 1)
    {
        transform_.position_.x -= RRay.dist;
        flyFlag = false;
    }
    /*for (int i = 0; i < 4; i++)
    {
        XMVector3TransformCoord(rayDir[i + 2], matCam);
    }*/
    
    /*Ray.start = transform_.position_;
    for (XMVECTOR check : rayDir)
    {

        XMStoreFloat3(&Ray.dir,check);
        Model::RayCast(stageNum_, Ray);
        if (Ray.dist <= 1)
        {
            transform_.position_.x -= Ray.dir.x*Ray.dist;
            transform_.position_.y -= Ray.dir.y*Ray.dist;
            transform_.position_.z -= Ray.dir.z*Ray.dist;
            flyFlag = false;
        }
    }*/
}

void Player::OnCollision(GameObject* pTarget)
{
    //KillMe();
}