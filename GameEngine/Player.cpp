#include "Player.h"
#include"Engine/Input.h"
#include"Engine/Model.h"
#include"Engine/Camera.h"
#include"Stage1.h"
#include"Pointer.h"
#include"Engine/SceneManager.h"
#include"EngineTime.h"
#include"Engine/Math.h"
#include"EnemyNormal.h"
#include"Easing.h"
#include<list>

std::vector<EnemyNormal*> enemyList_;

//コンストラクタ
Player::Player(GameObject* parent)
    :GameObject(parent, "Player"),
    baseUpVec_(XMVectorSet(0, 1, 0, 0)),
    playerLife_(10),
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
    velocity_(0),
    rotateSpeed_(4.0f),
    maxSpeed_(1.5f),
    wireLength_(100.0f),
    angleY_(0),
    angleX_(0),
    lockOnAngleLimit_(0.2f),
    flyFlag_(false),
    aimFlag_(false),
    groundFlag_(true)
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
    
    Instantiate<Pointer>(GetParent());

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
    Model::SetModelNum(stageNum_);

    if (firstRay.hit)
    {
        transform_.position_.y -= firstRay.dist-transform_.scale_.y;
    }
}

//更新
void Player::Update()
{
    rotateSpeed_ = 4.0f;
    EnemyNormal* pEnemy = (EnemyNormal*)FindObject("EnemyNormal");
    vPlayerPos_   = XMLoadFloat3(&transform_.position_);
    XMVECTOR vFly = XMVectorSet(0, 0, 0, 0);

    Pointer* pPointer = (Pointer*)FindObject("Pointer");
    pPointer->SetDraw(false);
    RayCastData ray;

    aimFlag_ = false;
    //トリガーを引くと移動できる壁にマーカーが表示される
    if (Input::GetLTrigger())
    {
        //レイキャストの判定距離の上限
        ray.distLimit = 100.0f;
        aimFlag_ = true;
        //当たる位置の計算
        XMVECTOR vPlayerDir = XMVector3TransformCoord(vBaseTarget_, matCamY_ * matCamX_);
        XMVECTOR vPtrDir = vPlayerDir;

        //エイムアシスト範囲内かどうか判定
        if (IsAssistRange(vPlayerDir, pEnemy->GetTransform().position_,ray.distLimit))
        {
           vPtrDir=XMVector3TransformCoord(vPtrDir,LookAtMatrix(pEnemy->GetTransform().position_, vPtrDir));
        }
        
        //レイキャストの始点と方向を入力
        XMStoreFloat3(&ray.start, vPlayerPos_);
        XMStoreFloat3(&ray.dir, vPtrDir);
        Model::RayCast(ray);

        //当たった位置にマーカー表示
        if (ray.hit && !flyFlag_)
        {
            rotateSpeed_ = 2.0f;
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
            vFlyMove_ = XMVector3Normalize(ray.hitPos - vPlayerPos_)* maxSpeed_;
            SetStatus(pPointer->GetObjectType());
        }
    }
    //当たってなかったらジャンプ
    else if(Input::IsPadButtonDown(XINPUT_GAMEPAD_A)&&airFlag_==false)
    {
        //ワイヤーで飛んでたらjumpFlag_はfalseにし、そうでなければtrue
        jumpFlag_ = flyFlag_ == true ? false : true;
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
        moveX = Input::GetLStick_X();
        moveZ = Input::GetLStick_Y();
        if (airFlag_ == true && jumpFlag_ == false)
        {
            moveX *= 0.3f;
            moveZ *= 0.3f;
        }
        if (groundFlag_ != true)
        {
            velocity_ += gravity_;
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


    vPlayerMove_  = vMove;
    velocity_     = max(velocity_, -2);
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
        flyMove_ = { 0, 0, 0 };
    }
    angleX_ += -Input::GetRStick_Y() * rotateSpeed_;
    angleY_ += Input::GetRStick_X() * rotateSpeed_;

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
   /* RayCastData FRay;
    RayCastData BRay;
    RayCastData LRay;
    RayCastData RRay;*/
    RayCastData URay;
    RayCastData DRay;


    /*FRay.start = transform_.position_;  
    BRay.start = transform_.position_;
    LRay.start = transform_.position_;
    RRay.start = transform_.position_;*/
    URay.start = transform_.position_;
    DRay.start = transform_.position_;
    
    /*XMStoreFloat3(&FRay.dir, rayDir_[DIR_FRONT]);
    XMStoreFloat3(&BRay.dir, rayDir_[DIR_BACK]);
    XMStoreFloat3(&LRay.dir, rayDir_[DIR_LEFT]);
    XMStoreFloat3(&RRay.dir, rayDir_[DIR_RIGHT]);    */
    XMStoreFloat3(&URay.dir, rayDir_[DIR_UP]);    
    XMStoreFloat3(&DRay.dir, rayDir_[DIR_DOWN]);    

    /*Model::RayCast(stageNum_, FRay);
    Model::RayCast(stageNum_, BRay);
    Model::RayCast(stageNum_, LRay);
    Model::RayCast(stageNum_, RRay);*/
    Model::RayCast(stageNum_, URay);
    Model::RayCast(stageNum_, DRay);

    XMFLOAT3 moveDist;
    float moveY;

    XMStoreFloat3(&moveDist,moveVector);
    moveY = moveDist.y;
    moveDist.y = 0;                         //ベクトルのy軸を0にする
    XMVECTOR moveHolizon = XMLoadFloat3(&moveDist);
    XMVECTOR startVec[3] = { 0 };
    startVec[0] = -XMVector3Normalize(moveHolizon);                                                    //進行方向
    startVec[1] = -XMVector3Rotate(-startVec[0],XMQuaternionRotationNormal(baseUpVec_, 0.5 * M_PI));   //進行方向に見て右
    startVec[2] = -XMVector3Rotate(-startVec[0], XMQuaternionRotationNormal(baseUpVec_, -0.5f * M_PI));//進行方向に見て左
    XMVECTOR wallzuri = XMVectorSet(0, 0, 0, 0);
   
    //進行方向のレイ
    RayCastData fMoveRay;
    XMStoreFloat3(&fMoveRay.start, vPlayerPos_+startVec[0]);
    XMStoreFloat3(&fMoveRay.dir, moveVector);
    Model::RayCast(stageNum_, fMoveRay);

    //進行方向に見て右のベクトル
    RayCastData lMoveRay;
    XMStoreFloat3(&lMoveRay.start, vPlayerPos_ + startVec[1]);
    XMStoreFloat3(&lMoveRay.dir, XMVector3Rotate(moveVector,XMQuaternionRotationNormal(-baseUpVec_,-0.5*M_PI)));
    Model::RayCast(stageNum_, lMoveRay);

    //進行方向に見て左ベクトル
    RayCastData rMoveRay;
    XMStoreFloat3(&rMoveRay.start, vPlayerPos_ + startVec[2]);
    XMStoreFloat3(&rMoveRay.dir, XMVector3Rotate(moveVector, XMQuaternionRotationNormal(-baseUpVec_,(0.5f*M_PI))));
    Model::RayCast(stageNum_, rMoveRay);
    float da = XMVectorGetX(XMVector3Length(moveHolizon));
    if (fMoveRay.dist < 2.0f)
    {
        vPlayerPos_ = XMLoadFloat3(&transform_.position_);
        moveDist = { 0,0,0 };
        //壁ズリベクトル = レイが当たったポリゴンの法線*進行方向ベクトルと法線の内積
        wallzuri = moveHolizon + (fMoveRay.normal * (1-XMVectorGetX(XMVector3Dot(-moveHolizon, fMoveRay.normal))));
        XMVECTOR back = (XMLoadFloat3(&fMoveRay.start) + (XMLoadFloat3(&fMoveRay.dir) * 2)) -fMoveRay.hitPos;
        XMStoreFloat3(&transform_.position_, vPlayerPos_+(-back));

        int a = 10;
    }

    if(lMoveRay.dist < 2.0f)
    {
        moveDist = { 0,0,0 };
        wallzuri = moveHolizon + (lMoveRay.normal * (1 - XMVectorGetX(XMVector3Dot(-moveHolizon, lMoveRay.normal))));
        XMVECTOR back = (XMLoadFloat3(&lMoveRay.start) + (XMLoadFloat3(&lMoveRay.dir) * 2)) - lMoveRay.hitPos;
        XMStoreFloat3(&transform_.position_, vPlayerPos_ + (-back));
    }

    if(rMoveRay.dist < 2.0f)
    {
        moveDist = { 0,0,0 };
        wallzuri = moveHolizon + (rMoveRay.normal * (1 - XMVectorGetX(XMVector3Dot(-moveHolizon, rMoveRay.normal))));
        XMVECTOR back = (XMLoadFloat3(&rMoveRay.start) + (XMLoadFloat3(&rMoveRay.dir) * 2)) - rMoveRay.hitPos;
        XMStoreFloat3(&transform_.position_, vPlayerPos_ + (-back));
    }

    vPlayerPos_ = XMLoadFloat3(&transform_.position_);
    //上レイの距離(dist)が1以下になったらy軸の座標を戻す
    if (moveY + transform_.scale_.y >= URay.dist)
    {
        transform_.position_.y += URay.dist - transform_.scale_.y;
        moveDist.y = 0;
        vFlyMove_ = XMVectorSet(0, 0, 0, 0);
        velocity_ = 0;
        //airFlag_ = false;
        flyFlag_ = false;
    }
    //下レイの距離(dist)が1以下になったらy軸の座標を戻す
    if (abs(moveY - transform_.scale_.y) > DRay.dist)
    {
        XMFLOAT3 pos;
        XMStoreFloat3(&pos, DRay.hitPos);
        //pos.y += transform_.scale_.y;
        transform_.position_.y += DRay.dist-transform_.scale_.y;
        //vPlayerPos_ = XMLoadFloat3(&transform_.position_);
        moveDist.y = 0;
        flyFlag_ = false;
        airFlag_ = false;
        jumpFlag_ = false;
        groundFlag_ = true;
    }
    else
    {
        groundFlag_ = false;
        //velocity_ = 0;
    }

    moveDist.y = moveY;
    moveVector = XMLoadFloat3(&moveDist);
    
    moveVector += wallzuri;
}

void Player::SetStatus(int type)
{
    status_ |= type;
}

void Player::OnCollision(GameObject* pTarget)
{
    if (pTarget->GetObjectName() == "EnemyNormal")
    {
        if (status_ & ATC_ATTACK)
        {
            flyFlag_ = false;
            vFlyMove_ = XMVector3TransformCoord(XMVector3Normalize(XMVectorSet(0, 1, -1, 0)), matCamX_) * 1.5f;
            //pTarget->KillMe();
        }
    }
}

bool Player::IsAssistRange(XMVECTOR dirVec,XMFLOAT3 targetPos, float length)
{
    
    XMVECTOR targetVec = XMLoadFloat3(&targetPos) -
                         XMLoadFloat3(&transform_.position_); //自分からtargetPosまでのベクトル
    
    //targetVecがlength以下だったら
    if (XMVectorGetX(XMVector3Length(targetVec)) < length)
    {
        targetVec = XMVector3Normalize(targetVec);
        dirVec = XMVector3Normalize(dirVec);
        float angle = 
            XMVectorGetX(XMVector3AngleBetweenNormals(dirVec, targetVec));     //targetVecとdirVecの内積を求める


            //angle(ラジアン)が±0.4の時カメラの回転速度を遅くする
        if (angle > -0.4f && angle < 0.4f)
        {
            rotateSpeed_ = rotateSpeed_ * angle + 0.6f;
        }

        //angle(ラジアン)がlockOnAngleLimit_いないだったらロックオン
        if (angle > -lockOnAngleLimit_ && angle < lockOnAngleLimit_)
        {
            return true;
        }
    }
    return false;
}