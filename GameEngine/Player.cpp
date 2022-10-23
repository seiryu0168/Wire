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
    //vPlayerMove(XMVectorSet(0,0,0,0)),
    vBaseTarget(XMVectorSet(0,0,5,0)),
    matCamX(XMMatrixIdentity()),
    matCamY(XMMatrixIdentity()),
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
    startPos.y += 10;*/
    RayCastData firstRay;
    firstRay.start = transform_.position_;
    
    rayDir[0] = XMVectorSet( 0, 0, 1, 0);
    rayDir[1] = XMVectorSet( 0, 0,-1, 0);
    rayDir[2] = XMVectorSet(-1, 0, 0, 0);
    rayDir[3] = XMVectorSet( 1, 0, 0, 0);
    rayDir[4] = XMVectorSet( 0, 1, 0, 0);
    rayDir[5] = XMVectorSet( 0,-1, 0, 0);
    
    XMStoreFloat3(&firstRay.dir,rayDir[DIR_DOWN]);
    Model::RayCast(stageNum_, firstRay);

    if (firstRay.hit)
    {
        transform_.position_.y -= firstRay.dist-transform_.scale_.y;
    }
}

//更新
void Player::Update()
{
 
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
        XMVECTOR vPtrBack = XMVector3TransformCoord(vBaseTarget, matCamY*matCamX);//XMLoadFloat3(&ptrBack);
 
        RayCastData ray;
        XMStoreFloat3(&ray.start, vPlayerPos);
        XMStoreFloat3(&ray.dir, vPtrBack);
        Model::RayCast(stageNum_, ray);
            
        if (Input::IsPadButtonDown(XINPUT_GAMEPAD_A))
        {
            XMFLOAT3 nmlCheck;
            XMStoreFloat3(&nmlCheck,XMVector3Dot(-(XMLoadFloat3(&ray.dir)), ray.normal));
            float nmlX = acos(nmlCheck.x);
            float nmlY = acos(nmlCheck.y);
            float nmlZ = acos(nmlCheck.z);
            if (ray.hit && abs(nmlX) <= (M_PI/2) && abs(nmlY) <= (M_PI/2) && abs(nmlZ) <= (M_PI/2))
            {
                vFlyMove = XMVector3Normalize(ray.hitPos - vPlayerPos);
                //XMStoreFloat3(&transform_.position_, ray.hitPos);
                flyFlag = true;
            }
        }
    }
    XMVECTOR vMove;
    vMove = XMVectorSet(Input::GetLStick_X(), 0, Input::GetLStick_Y(), 0);
    vMove = XMVector3TransformCoord(vMove, matCamX);
   
    XMVECTOR vFly = XMVectorSet(0, 0, 0, 0);
    if (flyFlag)
    {
        vFly=vFlyMove;
    }
    else
    {
        vMove += XMVectorSet(0, -0.2f, 0, 0);
    }

    XMVECTOR vPlayerMove = XMVectorSet(0, 0, 0, 0);
    vPlayerMove = vMove + vFly;
    CharactorControll(vPlayerMove);
    XMStoreFloat3(&transform_.position_, vPlayerPos + vPlayerMove);
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
    transform_.rotate_.y = angleY;
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
    matCamY = XMMatrixRotationX(angleX * (M_PI / 180));
    matCamX = XMMatrixRotationY(angleY * (M_PI / 180));
    vMoveCam = XMVector3TransformCoord(vCamPos, matCamY*matCamX);
    vTarCam = XMVector3TransformCoord(vBaseTarget, matCamY*matCamX);
    
    Camera::SetTarget(vPlayerPos+vTarCam);
    Camera::SetPosition(vPlayerPos + vMoveCam);
}

void Player::CharactorControll(XMVECTOR &moveVector)
{
    RayCastData FRay;
    RayCastData BRay;
    RayCastData LRay;
    RayCastData RRay;
    RayCastData URay;
    RayCastData DRay;
    FRay.start = transform_.position_;
    BRay.start = transform_.position_;
    LRay.start = transform_.position_;
    RRay.start = transform_.position_;
    URay.start = transform_.position_;
    DRay.start = transform_.position_;
    
    XMStoreFloat3(&FRay.dir, rayDir[DIR_FRONT]);
    XMStoreFloat3(&BRay.dir, rayDir[DIR_BACK]);
    XMStoreFloat3(&LRay.dir, rayDir[DIR_LEFT]);
    XMStoreFloat3(&RRay.dir, rayDir[DIR_RIGHT]);    
    XMStoreFloat3(&URay.dir, rayDir[DIR_UP]);    
    XMStoreFloat3(&DRay.dir, rayDir[DIR_DOWN]);    

    Model::RayCast(stageNum_, FRay);
    Model::RayCast(stageNum_, BRay);
    Model::RayCast(stageNum_, LRay);
    Model::RayCast(stageNum_, RRay);
    Model::RayCast(stageNum_, URay);
    Model::RayCast(stageNum_, DRay);

    XMFLOAT3 moveDist;
    XMStoreFloat3(&moveDist,moveVector);

    //前方レイの距離(dist)が1以下になったらz軸の座標を戻す
    if (moveDist.z+transform_.scale_.z >= FRay.dist)
    {
        transform_.position_.z += FRay.dist - 1.0f;
        moveDist.z = FRay.dist-1.0f;
        flyFlag = false;
    }
    
    //後方レイの距離(dist)が1以下になったらz軸の座標を戻す
    if (abs(moveDist.z-transform_.scale_.z) >= BRay.dist)
    {
        transform_.position_.z -= BRay.dist + 1.0f;
        moveDist.z = 0;
        flyFlag = false;
    }

    //右レイの距離(dist)が1以下になったらx軸の座標を戻す
    if (moveDist.x+transform_.scale_.x >= RRay.dist)
    {
        transform_.position_.x += RRay.dist - 1.0f;
        moveDist.x = 0;
            flyFlag = false;
    }

    //左レイの距離(dist)が1以下になったらx軸の座標を戻す
    if (abs(moveDist.x-transform_.scale_.x) >= LRay.dist)
    {
        transform_.position_.z -= LRay.dist + 1.0f;
        moveDist.x = 0;
        flyFlag = false;
    }
    //上レイの距離(dist)が1以下になったらx軸の座標を戻す
    if (moveDist.y - transform_.scale_.y >= URay.dist)
    {
        transform_.position_.y += URay.dist + 1.0f;
        moveDist.y = 0;
        flyFlag = false;
    }

    //下レイの距離(dist)が1以下になったらx軸の座標を戻す
    if (abs(moveDist.y - transform_.scale_.y) >= DRay.dist)
    {
        transform_.position_.y -= DRay.dist + 1.0f;
        moveDist.y = 0;
        flyFlag = false;
    }

    moveVector = XMLoadFloat3(&moveDist);

    
}

void Player::OnCollision(GameObject* pTarget)
{
    //KillMe();
}