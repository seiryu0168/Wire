#include "Player.h"
#include"Engine/Input.h"
#include"Engine/Model.h"
#include"Engine/Camera.h"
#include"Engine/SceneManager.h"
#include"EngineTime.h"
#include"ImageManager.h"
#include"EnemyNormal.h"
#include"Engine/Particle.h"
#include"Easing.h"
#include"Stage1.h"
#include"Pointer.h"
#include"Wire.h"
#include<list>
namespace
{
    static const std::vector<EnemyNormal*> enemyList_;
    static const float hitdist_=2.001f;
}

//コンストラクタ
Player::Player(GameObject* parent)
    :GameObject(parent, "Player"),
    baseUpVec_(XMVectorSet(0, 1, 0, 0)),
    playerLife_(10),
    gravity_(-0.06),
    hModel_(-1),
    hAudio_(-1),
    hModel_Handle_(-1),
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
    SetTag("Player");
    hModel_ = ModelManager::Load("Assets\\TestBox.fbx");
    assert(hModel_ >= 0);
    hModel_Handle_ = ModelManager::Load("Assets\\wire.fbx");
    assert(hModel_Handle_ > 0);
    
    pParticle_ = Instantiate<Particle>(GetParent());
    pLine_ = new LineParticle;
    pWire_ = new LineParticle;
    pLine_->SetLineParameter(0.5f, 30,0.4f);
    pWire_->SetLineParameter(0.1f, 2);
    pLine_->Load("Assets\\Line.png");
    pWire_->Load("Assets\\Effect01.png");
    //pWire_ = Instantiate<Wire>(this);
    OBBCollider* pCollider = new OBBCollider(XMFLOAT3(1,1,1), false, false);
    AddCollider(pCollider);
    stageNum_ = ((Stage1*)GetParent()->FindChild("Stage1"))->GetModelHandle();
    
    Instantiate<Pointer>(GetParent());
    pPointer_ = (Pointer*)FindObject("Pointer");

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
    ModelManager::RayCast(stageNum_, firstRay);
    ModelManager::SetModelNum(stageNum_);

    if (firstRay.hit)
    {
        transform_.position_.y -= firstRay.dist-transform_.scale_.y;
    }

    int hPict_ = ImageManager::Load("Assets\\Life.png");
    ImageManager::SetImagePos(hPict_, { -500.0f, 400.0f, 0 });
    ImageManager::SetImageSize(hPict_, { 1.0f,0.5f,1.0f });
}

//更新
void Player::Update()
{
    rotateSpeed_ = 4.0f;
    vPlayerPos_   = XMLoadFloat3(&transform_.position_);
    XMVECTOR vFly = XMVectorSet(0, 0, 0, 0);

    pPointer_->SetDraw(false);
    RayCastData ray;
    CheckTargetList();
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
        XMStoreFloat3(&ray.start, vPlayerPos_);
        XMStoreFloat3(&ray.dir, vPtrDir);

        //エイムアシスト範囲内かどうか判定
        if (enemyList_.size() > 0)
        {
            Enemy* pEnemy = AimAssist(&ray);
            if (pEnemy != nullptr)
            {
                vPtrDir = XMVector3TransformCoord(vPtrDir, LookAtMatrix(pEnemy->GetTransform().position_, vPtrDir));
                XMStoreFloat3(&ray.dir, vPtrDir);
            }
        }
        
        //レイキャストの始点と方向を入力
        ModelManager::RayCast(ray);

        //当たった位置にマーカー表示
        if (ray.hit && !flyFlag_)
        {
            rotateSpeed_ = 2.0f;
            XMFLOAT3 pointerPos;
            XMStoreFloat3(&pointerPos, ray.hitPos);
            pPointer_->SetPointerPos(pointerPos);
            pPointer_->SetDraw(ray.hit);
            pWire_->AddPosition(transform_.position_);
            pWire_->AddPosition(pointerPos);
        }
    }

    //レイが壁などに当たってたらその方向に向かうベクトルを作る
    if (Input::GetRTrigger() && pPointer_->IsDraw())
    {
        if (ray.hit)
        {
            groundFlag_ = false;
            airFlag_ = false;
            flyFlag_ = true;
            flyTime_ = 1;
            transform_.position_.y += 0.2f;
            velocity_ = 0;
            vFlyMove_ = XMVector3Normalize(ray.hitPos - vPlayerPos_)* maxSpeed_;
            SetStatus(pPointer_->GetObjectType());
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
        groundFlag_ = false;
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
        if (airFlag_ == true && jumpFlag_ == false&& groundFlag_==false)
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
    pLine_->AddPosition(transform_.position_);
    
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
    ModelManager::SetTransform(hModel_, transform_);
    ModelManager::Draw(hModel_);

    pLine_->Draw();
    pWire_->Draw();
    
}

//開放
void Player::Release()
{
}

void Player::CameraMove(RayCastData ray)
{
    
    //照準を定めている時
    if (aimFlag_ == true)
    {
        aimTime_ += 0.05f;
        aimTime_ = min(aimTime_, 1);
    }
    else//(aimFlag_==false)
    {
        aimTime_ += -0.07f;
        aimTime_ = max(aimTime_, 0.5);
        flyMove_ = { 0, 0, 0 };
    }

    //ワイヤーで飛んでいる時
    if (flyFlag_ == true)
    {
        aimFlag_ = false;
        aimTime_ += -0.08f;
        aimTime_ = max(aimTime_, 0);
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
    RayCastData URay;
    RayCastData DRay;


    XMFLOAT3 moveDist;
    float moveY;

    XMStoreFloat3(&moveDist,moveVector);
    moveY = moveDist.y;
    moveDist.y = 0;                         //ベクトルのy軸を0にする
    XMVECTOR moveHolizon = XMLoadFloat3(&moveDist);
    XMVECTOR startVec[5] = { 0 };
    startVec[0] = -XMVector3Normalize(moveHolizon);                                                //進行方向
    startVec[1] = -XMVector3Rotate(-startVec[0],XMQuaternionRotationNormal(baseUpVec_, M_PI/2));   //進行方向に見て右
    startVec[2] = -XMVector3Rotate(-startVec[0], XMQuaternionRotationNormal(baseUpVec_, -(M_PI/2)));  //進行方向に見て左
    startVec[3] = baseUpVec_;                                                                      //上ベクトル
    startVec[4] = -baseUpVec_;                                                                     //下ベクトル
    XMVECTOR wallzuri = XMVectorSet(0, 0, 0, 0);
   
    //進行方向のレイ
    RayCastData fMoveRay;
    XMStoreFloat3(&fMoveRay.start, vPlayerPos_+startVec[0]);
    XMStoreFloat3(&fMoveRay.dir, moveHolizon);
    ModelManager::RayCast(stageNum_, fMoveRay);

    //進行方向に見て右のレイ
    RayCastData lMoveRay;
    XMStoreFloat3(&lMoveRay.start, vPlayerPos_ + startVec[1]);
    XMStoreFloat3(&lMoveRay.dir, XMVector3Rotate(moveHolizon,XMQuaternionRotationNormal(-baseUpVec_,-(M_PI/2))));
    ModelManager::RayCast(stageNum_, lMoveRay);

    //進行方向に見て左のレイ
    RayCastData rMoveRay;
    XMStoreFloat3(&rMoveRay.start, vPlayerPos_ + startVec[2]);
    XMStoreFloat3(&rMoveRay.dir, XMVector3Rotate(moveHolizon, XMQuaternionRotationNormal(-baseUpVec_,(M_PI/2))));
    ModelManager::RayCast(stageNum_, rMoveRay);
   
    XMStoreFloat3(&URay.start,vPlayerPos_+startVec[4]);
    XMStoreFloat3(&DRay.start, vPlayerPos_ + startVec[3]);
    
    XMStoreFloat3(&URay.dir, startVec[3]);    
    XMStoreFloat3(&DRay.dir, startVec[4]);    
    ModelManager::RayCast(stageNum_, URay);
    ModelManager::RayCast(stageNum_, DRay);
    float da = XMVectorGetX(XMVector3Length(moveHolizon));
    
    if (fMoveRay.dist < hitdist_)
    {
        vPlayerPos_ = XMLoadFloat3(&transform_.position_);
        moveDist = { 0,0,0 };

        //壁ズリベクトル = レイが当たったポリゴンの法線*進行方向ベクトルと法線の内積
        wallzuri = moveHolizon + (fMoveRay.normal * (1-XMVectorGetX(XMVector3Dot(-moveHolizon, fMoveRay.normal))));
        XMVECTOR back = (XMLoadFloat3(&fMoveRay.start) + (XMLoadFloat3(&fMoveRay.dir) * 2)) -fMoveRay.hitPos;
        XMStoreFloat3(&transform_.position_, vPlayerPos_+(-back));

        if (flyFlag_)
        {
            flyFlag_ = false;
        }
    }

    if(lMoveRay.dist < hitdist_)
    {
        moveDist = { 0,0,0 };
        wallzuri = moveHolizon + (lMoveRay.normal * (1 - XMVectorGetX(XMVector3Dot(-moveHolizon, lMoveRay.normal))));
        XMVECTOR back = (XMLoadFloat3(&lMoveRay.start) + (XMLoadFloat3(&lMoveRay.dir) * 2)) - lMoveRay.hitPos;
        XMStoreFloat3(&transform_.position_, vPlayerPos_ + (-back));

        if (flyFlag_)
        {
            flyFlag_ = false;
        }
    }

    if(rMoveRay.dist < hitdist_)
    {
        moveDist = { 0,0,0 };
        wallzuri = moveHolizon + (rMoveRay.normal * (1 - XMVectorGetX(XMVector3Dot(-moveHolizon, rMoveRay.normal))));
        XMVECTOR back = (XMLoadFloat3(&rMoveRay.start) + (XMLoadFloat3(&rMoveRay.dir) * 2)) - rMoveRay.hitPos;
        XMStoreFloat3(&transform_.position_, vPlayerPos_ + (-back));

        if (flyFlag_)
        {
            flyFlag_ = false;
        }
    }

    //上レイの距離(dist)が1以下になったらy軸の座標を戻す
    if (URay.dist < hitdist_)
    {
        moveDist = { 0,0,0 };
        wallzuri = moveVector + (URay.normal * (1 - XMVectorGetX(XMVector3Dot(-moveHolizon, URay.normal))));
        XMVECTOR back = (XMLoadFloat3(&URay.start) + (XMLoadFloat3(&URay.dir) * 2)) - URay.hitPos;
        XMStoreFloat3(&transform_.position_, vPlayerPos_ + (-back));
        if (flyFlag_)
        {
            flyFlag_ = false;
        }
    }

    //下レイの距離(dist)がmoveY以下になったらy軸の座標を戻す
    if (DRay.dist < hitdist_)
    {
        if (signbit(moveY))
        {
            transform_.position_.y = DRay.start.y + transform_.scale_.y - DRay.dist;
            moveY = 0;
            velocity_ = 0;
            airFlag_ = false;
            groundFlag_ = true;
            if (flyFlag_)
            {
                flyFlag_ = false;
            }
        }
    }
    else
        groundFlag_ = false;

    vPlayerPos_ = XMLoadFloat3(&transform_.position_);
    moveDist.y = moveY;
    moveVector = XMLoadFloat3(&moveDist);
    
    moveVector += wallzuri;
}

void Player::SetStatus(int type)
{
    status_ = type;
}

void Player::OccurParticle()
{
    EmitterData data;
    if (status_ & ATC_ATTACK)
    {
        XMVECTOR pos = XMLoadFloat3(&transform_.position_);
        data.textureFileName = "Assets\\Effect01.png";
        XMFLOAT3 particlePos;
        XMStoreFloat3(&particlePos, pos + (XMVector3Normalize(vPlayerMove_)));
        data.position = particlePos;
        data.positionErr = XMFLOAT3(0.2, 0, 0.2);
        data.delay = 0;
        data.number = 30;
        data.lifTime = 50.0f;
        data.acceleration = 0.98f;
        data.gravity = 0.0f;

        XMFLOAT3 particleDir;
        XMStoreFloat3(&particleDir, -XMVector3Normalize(vPlayerMove_));
        data.dir = particleDir;
        data.dirErr = XMFLOAT3(360.0f, 360.0f, 360.0f);
        data.firstSpeed = 0.9f;
        data.speedErr = 0.2f;
        data.size = XMFLOAT2(1.5f, 1.5f);
        data.sizeErr = XMFLOAT2(0.3f, 0.3f);
        data.scale = XMFLOAT2(0.98f, 0.98f);
        data.color = XMFLOAT4(1, 1, 1, 1);
        data.deltaColor = XMFLOAT4(0, 0, 0, -0.08);
        pParticle_->ParticleStart(data);
    }

}

void Player::AddTargetList(Enemy* target)
{
    for (auto itr = enemyList_.begin(); itr != enemyList_.end(); itr++)
    {
        if ((*itr) == target)
        {
            return;
        }
    }
        enemyList_.push_back(target);
}
void Player::OnCollision(GameObject* pTarget)
{
    if (pTarget->GetTag()=="Enemy")
    {
        if (status_ & ATC_ATTACK)
        {
            OccurParticle();
            flyFlag_ = false;
            XMStoreFloat3(&transform_.position_ ,vPlayerPos_);
            vFlyMove_ = -vFlyMove_;
        }
    }
}

void Player::CheckTargetList()
{
    for (auto itr = enemyList_.begin(); itr != enemyList_.end();)
    {
        if ((*itr)->GetIsList() == false)
        {
            itr = enemyList_.erase(itr);
            
        }
        else
        {
            itr++;
        }
        if (enemyList_.empty())
        {
            return;
        }
    }
}

void Player::DeleteTargetList(Enemy* target)
{
    for (auto itr = enemyList_.begin(); itr != enemyList_.end(); itr++)
    {
        if ((*itr) == target)
        {
            itr = enemyList_.erase(itr);
        }
        if (enemyList_.empty())
        {
            return;
        }
    }
}

bool Player::IsAssistRange(XMVECTOR dirVec,XMFLOAT3 targetPos, float length)
{
    
    XMVECTOR targetVec = XMLoadFloat3(&targetPos) -
                         XMLoadFloat3(&transform_.position_);                       //自分からtargetPosまでのベクトル
    
    //targetVecがlength以下だったら
    if (XMVectorGetX(XMVector3Length(targetVec)) < length)
    {
        targetVec = XMVector3Normalize(targetVec);
        dirVec = XMVector3Normalize(dirVec);
        float angle = XMVectorGetX(XMVector3AngleBetweenNormals(dirVec, targetVec));//targetVecとdirVecの内積を求める

        //angle(ラジアン)が±0.4の時カメラの回転速度を遅くする
        if (angle > -0.4f && angle < 0.4f)
        {
            rotateSpeed_ = rotateSpeed_ * angle + 0.6f;
        }

        //angle(ラジアン)がlockOnAngleLimit_以内だったらロックオン
        if (angle > -lockOnAngleLimit_ && angle < lockOnAngleLimit_)
        {
            return true;
        }
    }
    return false;
}

Enemy* Player::AimAssist(RayCastData* ray)
{
    CheckTargetList();
    if (enemyList_.empty())
        return nullptr;

    float minRange = 9999.0f;
    Enemy* pEnemy=nullptr;
    for (auto itr = enemyList_.begin(); itr != enemyList_.end(); itr++)
    {
        if (IsAssistRange(XMLoadFloat3(&ray->dir), (*itr)->GetPosition(), ray->distLimit))
        {
            XMFLOAT3 targetPos = (*itr)->GetPosition();
            float range = XMVectorGetX(XMVector3Length(XMLoadFloat3(&targetPos) - XMLoadFloat3(&transform_.position_)));
            if (range < minRange)
            {
                minRange = range;
                pEnemy = (*itr);
            }

        }
    }

    return pEnemy;
}