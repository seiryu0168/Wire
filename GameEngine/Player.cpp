#include "Player.h"
#include"Engine/Input.h"
#include"Engine/Model.h"
#include"Engine/Camera.h"
#include"Stage1.h"
#include"Pointer.h"
#include"Engine/SceneManager.h"
#include"EngineTime.h"
#include"Engine/Math.h"
#include"Easing.h"

//コンストラクタ
Player::Player(GameObject* parent)
    :GameObject(parent, "Player"),
    gravity_(-0.06),
    hModel_(-1),
    vCamPos_(XMVectorSet(0, 10, -30, 0)),
    vPlayerPos_(XMVectorSet(0, 0, 0, 0)),
    vBaseTarget_(XMVectorSet(0, 0, 80, 0)),
    vBaseAim_(XMVectorSet(3, 2, -4, 0)),
    matCamX_(XMMatrixIdentity()),
    matCamY_(XMMatrixIdentity()),
    moveTime_(0),
    aimTime_(0),
    flyTime_(0),
    velocity_(2),
    rotateSpeed_(4.0f),
    maxSpeed_(1.5f),
    angleY_(0),
    angleX_(0),
    flyFlag_(false),
    aimFlag_(false)
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
   
    BoxCollider* pCollider = new BoxCollider(XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1));
    AddCollider(pCollider);
    stageNum_ = ((Stage1*)GetParent()->FindChild("Stage1"))->GetModelHandle();
    
    Instantiate<Pointer>(this);

    transform_.position_ = XMFLOAT3(0, 20,0);
    RayCastData firstRay;
    firstRay.start = transform_.position_;
    
    rayDir_[0] = XMVectorSet( 0, 0, 1, 0);
    rayDir_[1] = XMVectorSet( 0, 0,-1, 0);
    rayDir_[2] = XMVectorSet(-1, 0, 0, 0);
    rayDir_[3] = XMVectorSet( 1, 0, 0, 0);
    rayDir_[4] = XMVectorSet( 0, 1, 0, 0);
    rayDir_[5] = XMVectorSet( 0,-1, 0, 0);
    
    XMStoreFloat3(&firstRay.dir,rayDir_[DIR_DOWN]);
    Model::RayCast(stageNum_, firstRay);

    if (firstRay.hit)
    {
        transform_.position_.y -= firstRay.dist-transform_.scale_.y;
    }
}

//更新
void Player::Update()
{
    vPlayerPos_   = XMLoadFloat3(&transform_.position_);
    XMVECTOR vFly = XMVectorSet(0, 0, 0, 0);

    Pointer* pPointer = (Pointer*)FindChild("Pointer");
    pPointer->SetDraw(false);
    RayCastData ray;

    aimFlag_ = false;
    //トリガーを引くと移動できる壁にマーカーが表示される
    if (Input::GetLTrigger())
    {
        aimFlag_ = true;
        //当たる位置の計算
        XMVECTOR vPtrBack = XMVector3TransformCoord(vBaseTarget_, matCamY_ * matCamX_);

        //レイキャストの始点と方向を入力
        XMStoreFloat3(&ray.start, vPlayerPos_);
        XMStoreFloat3(&ray.dir, vPtrBack);
        Model::RayCast(stageNum_, ray);
        
        //当たった位置にマーカー表示
        if (ray.hit)
        {
            XMFLOAT3 pointerPos;
            XMStoreFloat3(&pointerPos, ray.hitPos);
            pPointer->SetPointerPos(pointerPos);
            pPointer->SetDraw(ray.hit);
        }
    }

    //レイが壁などに当たってたらその方向に向かうベクトルを作る
    if (Input::GetRTrigger() && pPointer->IsDraw())
    {
        if (ray.hit)
        {
            pPointer->GetObjectType();
            airFlag_ = false;
            flyFlag_ = true;
            flyTime_ = 1;
            transform_.position_.y += 0.2f;
            velocity_ = 0;
            vFlyMove_ = XMVector3Normalize(ray.hitPos - vPlayerPos_)* rotateSpeed_;
        }
    }
    //当たってなかったらジャンプ
    else if(Input::IsPadButtonDown(XINPUT_GAMEPAD_A)&&airFlag_==false)
    {
        //ワイヤーで飛んでたらjumpFlag_はfalseにし、そうでなければtrue
        if (flyFlag_ == true)
        {
            jumpFlag_ = false;
        }
        else
            jumpFlag_ = true;
        //ジャンプするとワイヤーアクションは中断されるのでflyFlag_はfalse
        flyFlag_ = false;
        //空中にいるのでairFlag_はtrue
        airFlag_ = true;
        //垂直方向の速度変更
        velocity_ = 2;
        vFlyMove_ *= 0.3f;
        transform_.position_.y += 0.2f;
    }

    //左スティックの入力値を入れる変数
    float moveX=0;
    float moveZ=0;

    //重力加算
    vFly += XMVectorSet(0, velocity_, 0, 0);

    //ワイヤーで飛んでなければ移動と落下が出来る
    if (flyFlag_ == false)
    {
        velocity_ += gravity_;
        moveX = Input::GetLStick_X();
        moveZ = Input::GetLStick_Y();
        if (airFlag_ == true&&jumpFlag_==false)
        {
            moveX *= 0.3f;
            moveZ *= 0.3f;
        }
    }



    //L,Rスティックで移動
    XMVECTOR vMove = XMVectorSet(moveX, 0, moveZ, 0);
    //スティックが傾いてれば徐々に加速し、傾いてなければ徐々に減速
    if (abs(moveX) > 0 || abs(moveZ)>0)
    {
        moveTime_ += 0.1f;
        moveTime_=min(moveTime_, 1);
    }
    else
    {
        moveTime_ -= 0.05f;
        moveTime_=max(moveTime_, 0);
    }

    //ワイヤーで飛んでいれば徐々に加速し、飛んでなければ徐々に減速
    if (flyFlag_)
    {
        flyTime_ += 0.01f;
        flyTime_=min(flyTime_, 1);
    }
    else
    {
        flyTime_ -= 0.01f;
        flyTime_=max(flyTime_, 0);
    }

    //行列で移動のベクトルをカメラの向きに変形
    vMove = XMVector3TransformCoord(vMove, matCamX_);


    vPlayerMove_ = vMove;
    velocity_ = max(velocity_, -2);
    vPlayerMove_ += XMVectorLerp(XMVectorSet(0, 0, 0, 0), vFlyMove_, Easing::EaseOutQuad(flyTime_));
    vPlayerMove_ += vFly;
    CharactorControll(vPlayerMove_);
    XMStoreFloat3(&transform_.position_, vPlayerPos_+vPlayerMove_);
    CameraMove(ray);
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

void Player::CameraMove(RayCastData ray)
{
    if (flyFlag_ == true)
    {
        aimFlag_ = false;
        aimTime_ += -0.1;
        aimTime_ = max(aimTime_, 0);
    }
    if (aimFlag_)
    {
        aimTime_ += 0.05f;
        aimTime_ = min(aimTime_, 1);
    }
    else if(!flyFlag_)
    {
        aimTime_ -= 0.07f;
        aimTime_ = max(aimTime_, 0.5);
    }
    angleX_ += -Input::GetRStick_Y() * rotateSpeed_;
    angleY_ += Input::GetRStick_X() * rotateSpeed_;
    transform_.rotate_.y = angleY_;
    vPlayerPos_ = XMLoadFloat3(&transform_.position_);
    XMVECTOR vMoveCam;
    XMVECTOR vTarCam;
    XMVECTOR vNormalCam;
    XMVECTOR vAimCam;

    if (angleX_ <= -90)
    {
        angleX_ = -89;
    }
    if (angleX_ >= 70)
    {
        angleX_ = 69;
    }


    matCamY_   = XMMatrixRotationX(angleX_ * (M_PI / 180));
    matCamX_   = XMMatrixRotationY(angleY_ * (M_PI / 180));
    vNormalCam = XMVector3TransformCoord(vCamPos_,  matCamY_ * matCamX_);
    vAimCam    = XMVector3TransformCoord(vBaseAim_, matCamY_ * matCamX_);
    vTarCam    = vPlayerPos_+XMVector3TransformCoord(vBaseTarget_, matCamY_ * matCamX_);
    
    vMoveCam   = XMVectorLerp(vNormalCam, vAimCam, aimTime_);
    
    Camera::SetTarget(vTarCam);
    Camera::SetPosition(vPlayerPos_ + vMoveCam);
}

void Player::CharactorControll(XMVECTOR &moveVector)
{

    //XMFLOAT3 move;
    //XMStoreFloat3(&move, moveVector);
    //move.y = 0;
    //XMVECTOR da = XMLoadFloat3(&move);
    //XMVECTOR f = XMVector3TransformCoord(da,matCamX_);
    //XMVECTOR b = XMVector3TransformCoord(f, XMMatrixRotationY(M_PI));
    //XMVECTOR r = XMVector3TransformCoord(f, XMMatrixRotationY(M_PI * 0.5f));
    //XMVECTOR l = XMVector3TransformCoord(f, XMMatrixRotationY(-(M_PI * 0.5f)));

    //RayCastData front;
    ////RayCastData back;
    //RayCastData right;
    //RayCastData left;
    //XMStoreFloat3(&front.dir, f);
    ////MStoreFloat3(&back.dir, b);
    //XMStoreFloat3(&right.dir, r);
    //XMStoreFloat3(&left.dir, l);

    //Model::RayCast(stageNum_, front);
    ////Model::RayCast(stageNum_, back);
    //Model::RayCast(stageNum_, right);
    //Model::RayCast(stageNum_, left);


    //if (Math::IsFrontSurface(front.normal,front.hitPos-(vPlayerPos_+da)))
    //{
    //   XMStoreFloat3(&transform_.position_,vPlayerPos_+front.hitPos+front.normal);
    //}

   /* if (mV > XMVectorGetX(XMVector3Length(back.hitPos - vPlayerPos_)))
    {
        XMStoreFloat3(&transform_.position_, back.hitPos + back.normal);
    }

    if (mV > XMVectorGetX(XMVector3Length(right.hitPos - vPlayerPos_)))
    {
        XMStoreFloat3(&transform_.position_, right.hitPos + right.normal);
    }

    if (mV > XMVectorGetX(XMVector3Length(left.hitPos - vPlayerPos_)))
    {
        XMStoreFloat3(&transform_.position_, left.hitPos + left.normal);
    }*/




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

   /* FRay.start.z -= 0.5f;
    BRay.start.z += 0.5f;
    LRay.start.x += 0.5f;
    RRay.start.x -= 0.5f;*/
    
    XMStoreFloat3(&FRay.dir, rayDir_[DIR_FRONT]);
    XMStoreFloat3(&BRay.dir, rayDir_[DIR_BACK]);
    XMStoreFloat3(&LRay.dir, rayDir_[DIR_LEFT]);
    XMStoreFloat3(&RRay.dir, rayDir_[DIR_RIGHT]);    
    XMStoreFloat3(&URay.dir, rayDir_[DIR_UP]);    
    XMStoreFloat3(&DRay.dir, rayDir_[DIR_DOWN]);    

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
        //transform_.position_.z -= 1.51f - FRay.dist;// -transform_.scale_.z;
        //XMStoreFloat3(&transform_.position_, FRay.hitPos);
        //transform_.position_.z -= 1.6f;
        moveDist.z = 0;
        vFlyMove_ = XMVectorSet(0,0,0,0);
        //airFlag_ = false;
        flyFlag_ = false;
    }
        if (FRay.dist < 1.0f)
        {
            transform_.position_.z -= 1.0f-FRay.dist;
        }
        vPlayerPos_ = XMLoadFloat3(&transform_.position_);
    //後方レイの距離(dist)が1以下になったらz軸の座標を戻す
    if (abs(moveDist.z-transform_.scale_.z) >= BRay.dist || BRay.dist < 1.5f)
    {
        //transform_.position_.z -= BRay.dist + transform_.scale_.z;
        moveDist.z = 0;
        vFlyMove_ = XMVectorSet(0, 0, 0, 0);
        //airFlag_ = false;
        flyFlag_ = false;
    }

    if (BRay.dist < 1.0f)
    {
        transform_.position_.z += 1.0f - BRay.dist;
    }

    //右レイの距離(dist)が1以下になったらx軸の座標を戻す
    if (moveDist.x+transform_.scale_.x >= RRay.dist || RRay.dist < 1.5f)
    {
        //transform_.position_.x += RRay.dist - transform_.scale_.x;
        moveDist.x = 0;
        vFlyMove_ = XMVectorSet(0, 0, 0, 0);
        //airFlag_ = false;
        flyFlag_ = false;
    }

    if (RRay.dist < 1.0f)
    {
        transform_.position_.x -= 1.0f - RRay.dist;
    }

    //左レイの距離(dist)が1以下になったらx軸の座標を戻す
    if (abs(moveDist.x-transform_.scale_.x) >= LRay.dist || LRay.dist < 1.5f)
    {
        //transform_.position_.x -= LRay.dist + transform_.scale_.x;
        moveDist.x = 0;
        vFlyMove_ = XMVectorSet(0, 0, 0, 0);
        //airFlag_ = false;
        flyFlag_ = false;
    }

    if (LRay.dist < 1.0f)
    {
        transform_.position_.x += 1.0f - LRay.dist;
    }

    //上レイの距離(dist)が1以下になったらx軸の座標を戻す
    if (moveDist.y + transform_.scale_.y >= URay.dist)
    {
        transform_.position_.y += URay.dist - transform_.scale_.y;
        moveDist.y = 0;
        vFlyMove_ = XMVectorSet(0, 0, 0, 0);
        velocity_ = 0;
        //airFlag_ = false;
        flyFlag_ = false;
    }

    //下レイの距離(dist)が1以下になったら軸の座標を戻す
    if (abs(moveDist.y-transform_.scale_.y) > DRay.dist)
    {
        XMFLOAT3 pos;
        XMStoreFloat3(&pos, DRay.hitPos);
        pos.y += transform_.scale_.y;
        transform_.position_ = pos;
        vPlayerPos_ = XMLoadFloat3(&transform_.position_);
        moveDist.y = 0;
        flyFlag_ = false;
        airFlag_ = false;
        jumpFlag_ = false;
        //vFlyMove_ = XMVectorSet(0, 0, 0, 0);
        //velocity_ = 0;
    }
    //vPlayerPos_ = XMLoadFloat3(&transform_.position_);
    moveVector = XMLoadFloat3(&moveDist);

    
}

void Player::SetColor(short type)
{
    switch (type)
    {
    case ATC_ATTACK:

        break;
    case ATC_TOWALL:
    }
}


void Player::OnCollision(GameObject* pTarget)
{
    if (pTarget->GetObjectName() == "EnemyNormal")
    {
       // if()
    }
}