#include"Engine/DirectX_11/Input.h"
#include"Engine/ResourceManager/Model.h"
#include"Engine/GameObject/Camera.h"
#include"Engine/SceneManager.h"
#include"Engine/Collider/SphereCollider.h"
#include"EngineTime.h"
#include"Engine/ResourceManager/ImageManager.h"
#include"Engine/DirectX_11/Particle.h"
#include"PlayerBase.h"
#include"Engine/ResourceManager/Audio.h"
//#include"ModelComponent.h"
#include"ItemGetter.h"
#include"PlayScreen.h"
#include"Player.h"
#include"InterSceneData.h"
#include"ObjectSetter.h"
#include"EnemyNormal.h"
#include"Easing.h"
#include"Stage1.h"
#include"Pointer.h"
#include"Engine/DirectX_11/Math.h"
#include"Wire.h"

#include<list>
namespace
{
    static const std::vector<Enemy*> enemyList_;
    static const float hitdist_   =3.001f;
    static const int   MAX_LIFE   = 10;
    static const int   MAX_GODTIME = 30;
    static const float DELTA_AIM_TIME = 0.05f;
    //体力の定数
    static const float LIFE_OFFSET_X = -1800.0f;
    static const float LIFE_OFFSET_Y = -900.0f;
    static const int   LIFE_INTERVAL = 105;
    static const XMFLOAT3 LIFE_SIZE = { 0.2f,0.2f,1.0f };
    //カメラの定数
    static const float ACCEL_AOV     = 70.0f;
    static const float NORMAL_AOV    = 45.0f;
    static const float CAMERA_DIST   = 1.5f;
    static const float SHAKE_RATE = 1.5f;
    static const float ANGLE_MAX     = 69.0f;
    static const float ANGLE_MIN     = -89.0f;
    static const float CAMERA_ROTATESPEED_NORMAL = 4.0f;
    static const float LOW_CAMERA_ROTATE_SPEED = 0.55f;

    static const float AIM_ASSIST_ANGLE = XMConvertToRadians(30.0f);
    static const float ATTACK_SPEED = 1.8f;
    static const float MAX_VELOCITY = 2.0f;
    static const float DELTA_FLY_TIME = 0.01f;
    static const float LOW_SPEED = 0.3f;
    static const float ASSISTLIMIT = 100.0f;
    static const float RAY_OFFSET = 2.0f;
    static const float LOW_LIMIT = -100.0f;

    static const XMFLOAT4 LINECOLOR_RED = { 1,0,0,1 };
    static const XMFLOAT4 LINECOLOR_DEFAULT = { 1,1,1,1 };
}

//コンストラクタ
Player::Player(GameObject* parent)
    :GameObject(parent, "Player"),
    baseUpVec_(XMVectorSet(0, 1, 0, 0)),
    playerLife_(MAX_LIFE),
    prevHitBit_(0),
    gravity_(-0.06f),
    hModel_(-1),
    hAudio_(-1),
    hAudioShoot_(-1),
    stageNum_(-1),
    prevPositionVec(XMVectorZero()),
    vCamPos_(XMVectorSet(0, 10, -30, 0)),
    vPlayerPos_(XMVectorZero()),
    vBaseTarget_(XMVectorSet(0, 0, 80, 0)),
    vBaseAim_(XMVectorSet(3, 2, -4, 0)),
    vFlyMove_(XMVectorZero()),
    vPlayerMove_(XMVectorZero()),
    cameraShake_(XMVectorZero()),
    matCamX_(XMMatrixIdentity()),
    matCamY_(XMMatrixIdentity()),
    moveTime_(0),
    aimTime_(0),
    flyTime_(0),
    godTime_(0),
    velocity_(0),
    rotateSpeed_(4.0f),
    speed_(1),
    maxSpeed_(2.0f),
    wireLength_(100.0f),
    angleY_(0),
    angleX_(0),
    lockOnAngleLimit_(0.2f),
    lockOn_(false),
    flyFlag_(false),
    airFlag_(false),
    aimFlag_(false),
    jumpFlag_(false),
    groundFlag_(true),
    godFlag_(false),
    pPointerLine_(nullptr),
    pParticle_(nullptr),
    pPointer_(nullptr),
    pSetter_(nullptr),
    wire_(nullptr)
{
    Instantiate<PlayerBase>(this);
    pScreen_ = Instantiate<PlayScreen>(this);
    //パーティクルオブジェクト生成
    pParticle_ = Instantiate<Particle>(this);
}

//デストラクタ
Player::~Player()
{
    Release();
}

//初期化
void Player::Initialize()
{
    pItemGetter_ = new ItemGetter(this);
    //タグ設定
    SetTag("Player");
    //セッターのポインター取得
    pSetter_ = (ObjectSetter*)FindObject("ObjectSetter");
    
    //モデルロード
    hModel_ = ModelManager::Load("Assets\\WireShooter_Maya.fbx");
    assert(hModel_ >= 0);

    //マーカーを生成
    pPointer_=Instantiate<Pointer>(GetParent());
    
    //ラインパーティクル生成
    pPointerLine_ = new LineParticle;
    //パラメータ設定
    pPointerLine_->SetLineParameter(0.1f, 2);
    //ラインパーティクル用画像のロード
    pPointerLine_->Load("Assets\\Effect01.png");

    //衝突音
    hAudio_ = Audio::Load("Assets\\explosion.wav", 10);
    assert(hAudio_ >= 0);
    
    //射出音
    hAudioShoot_ = Audio::Load("Assets\\shoot.wav",5);
    assert(hAudioShoot_ >= 0);
    
    //当たり判定
    SphereCollider* pCollider = new SphereCollider(XMFLOAT3(0,0,0),2);
    AddCollider(pCollider);
    stageNum_ = ((Stage*)GetParent()->FindChild("Stage1"))->GetModelHandle();
    areaLimit_ = ((Stage*)GetParent()->FindChild("Stage1"))->GetStageAreaLimit();
    pPointer_->SetPosition({ 9999.0f,9999.0f,9999.0f });
    transform_.position_ = XMFLOAT3(0, 20,0);
   
    //初期位置用のレイキャスト準備
    RayCastData firstRay;
    firstRay.start = transform_.position_;
       
    
    XMStoreFloat3(&firstRay.dir, XMVectorSet(0, -1, 0, 0));
    ModelManager::RayCast(stageNum_, firstRay);
    ModelManager::SetModelNum(stageNum_);

    if (firstRay.hit)
    {
        transform_.position_.y -= firstRay.dist-transform_.scale_.y;
    }

    wire_ = new Wire;
    for (int i = 0; i < MAX_LIFE; i++)
    {
        int hPict_ = ImageManager::Load("Assets\\LifeImage.png");
        ImageManager::SetImagePos(hPict_, { LIFE_OFFSET_X+ LIFE_INTERVAL *i, LIFE_OFFSET_Y, 0 });
        ImageManager::SetImageSize(hPict_, LIFE_SIZE);
        ImageManager::SetUIList(hPict_);
        life_.push_back(hPict_);
    }
}

//更新
void Player::Update()
{
    //無敵時間の処理
    if (godFlag_)
    {
        cameraShake_ = XMVectorSet((float)(rand() % 10)/10, (float)(rand() % 10) / 10, 0, 0)*SHAKE_RATE;
        godTime_--;
        godTime_ = max(godTime_,0);
    }
    if(godTime_==0)
    {
        cameraShake_ = XMVectorZero();
        godFlag_ = false;
    }

    //回転速度やらポジションの取得やらの処理
    rotateSpeed_ = CAMERA_ROTATESPEED_NORMAL;
    vPlayerPos_   = XMLoadFloat3(&transform_.position_);
    XMVECTOR vFly = XMVectorZero();

    pPointer_->SetDraw(false);
    RayCastData ray;
    CheckTargetList();
    aimFlag_ = false;
    
    //トリガーを引くと移動できる壁にマーカーが表示される
    if (Input::GetLTrigger())
    {
        Aim(&ray);
    }
    else
    {
        enemyNumber_ = -1;
        lockOn_ = false;
    }

    //レイが壁などに当たってたらその方向に向かうベクトルを作る
    if (Input::GetRTriggerDown() && pPointer_->IsDraw())
    {
        if (ray.hit)
        {
            groundFlag_ = false;
            airFlag_ = false;
            flyFlag_ = true;
            //flyTime_ = 1;
            velocity_ = 0;
            wire_->ShotWire(vPlayerPos_, ray.hitPos);
            Audio::Play(hAudioShoot_);
        }
        
    }
    wire_->Update();
    if (wire_->GetWireState()==WIRE_STATE::EXTEND)
    {
        vFlyMove_ = wire_->GetWireVec() * maxSpeed_;
        SetStatus(ATC_ATTACK);
        
        pPointer_->SetPosition({ 9999.0f,9999.0f,9999.0f });
    }
    else if (wire_->GetWireState() == WIRE_STATE::STRETCH)
    {
        return;
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
        velocity_ = MAX_VELOCITY;
        vFlyMove_ *= LOW_SPEED;
        //transform_.position_.y += 0.2f;
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

        //地面についてない時重力が働く
        if (groundFlag_ != true)
        {
            velocity_ += gravity_;
        }
    }
    pScreen_->SetDir({ moveX,moveZ });

    //L,Rスティックで移動
    XMVECTOR vMove = XMVectorSet(moveX, 0, moveZ, 0);
    
    //ワイヤーで飛んでいれば徐々に加速し、飛んでなければ徐々に減速
   
    if(flyFlag_==false)
    {
        flyTime_ -= DELTA_FLY_TIME;
        flyTime_=max(flyTime_, 0);
    }
    else
    {
        flyTime_ += DELTA_FLY_TIME*10;
        flyTime_ = min(flyTime_, 1);
    }
    
    //行列で移動のベクトルをカメラの向きに変形
    vMove = XMVector3TransformCoord(vMove, matCamX_);

    //移動
    vPlayerMove_  = vMove;
    velocity_     = max(velocity_, -MAX_VELOCITY);
    vPlayerMove_ += XMVectorLerp(XMVectorZero(), vFlyMove_, Easing::EaseOutQuad(flyTime_));
    vPlayerMove_ += vFly;
    CharactorControll(vPlayerMove_);
    CameraMove(ray);
    transform_.rotate_.y = angleY_;
    pItemGetter_->Update();
}

//描画
void Player::Draw()
{
    //((ModelComponent*)GetComponent(0))->SetTransform(&transform_);
    ModelManager::SetTransform(hModel_, transform_);
    ModelManager::Draw(hModel_); 
}

void Player::SecondDraw()
{
    wire_->Draw(transform_);
    if (pPointer_->IsDraw())
    {
        if (lockOn_)
        {
            pPointerLine_->SetColor(LINECOLOR_RED);
        }
        else
            pPointerLine_->SetColor(LINECOLOR_DEFAULT);
        pPointerLine_->Draw(&transform_);
    }
}

//開放
void Player::Release()
{
    SAFE_DELETE(pItemGetter_);
    SAFE_DELETE(wire_);
    SAFE_RELEASE(pPointerLine_);
}

void Player::SetAreaLimit(XMFLOAT4 limit)
{
    areaLimit_ = limit;
}

void Player::CameraMove(RayCastData ray)
{
    
    //照準を定めている時
    if (aimFlag_ == true)
    {
        aimTime_ += DELTA_AIM_TIME;
        aimTime_ = (float)min(aimTime_, 1.0f);
    }
    else
    {
        aimTime_ += -DELTA_AIM_TIME;
        aimTime_ = (float)max(aimTime_, 0.5f);
    }

    //ワイヤーで飛んでいる時
    if (flyFlag_ == true)
    {
        aimFlag_ = false;
        aimTime_ += -DELTA_AIM_TIME;
        aimTime_ = (float)max(aimTime_, 0);
    }

    //カメラの画角
    float AOVRate = max(NORMAL_AOV,flyTime_ * ACCEL_AOV);
    Camera::SetAOV(XMConvertToRadians(AOVRate));
    angleX_ += -Input::GetRStick_Y() * rotateSpeed_;
    angleY_ += Input::GetRStick_X() * rotateSpeed_;
    
    //
    vPlayerPos_ = XMLoadFloat3(&transform_.position_);
    XMVECTOR vMoveCam;
    XMVECTOR vTarCam;
    XMVECTOR vNormalCam;
    XMVECTOR vAimCam;

    if (angleX_ <= ANGLE_MIN)
    {
        angleX_ = ANGLE_MIN;
    }
    if (angleX_ >= ANGLE_MAX)
    {
        angleX_ = ANGLE_MAX;
    }

    //回転行列作成
    matCamY_   = XMMatrixRotationX(XMConvertToRadians(angleX_));
    matCamX_   = XMMatrixRotationY(XMConvertToRadians(angleY_));
    
    //カメラの基準となる位置ベクトル
    vNormalCam = vCamPos_*  matCamY_ * matCamX_* CAMERA_DIST;
    //エイム時のベクトル
    vAimCam    = vBaseAim_* matCamY_ * matCamX_;
    //焦点
    vTarCam    = vPlayerPos_+ vBaseTarget_ * matCamY_* matCamX_ +cameraShake_;
    
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
    XMVECTOR startVec[(int)DIRECTION::DIR_MAX] = { 0 };
    startVec[(int)DIRECTION::DIR_FRONT] = -XMVector3Normalize(moveHolizon);                                           //進行方向
    startVec[(int)DIRECTION::DIR_RIGHT] = -XMVector3Rotate(-startVec[(int)DIRECTION::DIR_FRONT],XMQuaternionRotationNormal(baseUpVec_, (float)(M_PI/2.0f)));   //進行方向に見て右
    startVec[(int)DIRECTION::DIR_LEFT] = -XMVector3Rotate(-startVec[(int)DIRECTION::DIR_FRONT], XMQuaternionRotationNormal(baseUpVec_, -(float)(M_PI/2.0f)));  //進行方向に見て左
    startVec[(int)DIRECTION::DIR_UP] = baseUpVec_;                                                                      //上ベクトル
    startVec[(int)DIRECTION::DIR_DOWN] = -baseUpVec_;                                                                     //下ベクトル
    XMVECTOR wallzuri = XMVectorZero();
   
    //進行方向のレイ
    RayCastData fMoveRay;
    XMStoreFloat3(&fMoveRay.start, vPlayerPos_+startVec[(int)DIRECTION::DIR_FRONT]);
    XMStoreFloat3(&fMoveRay.dir, moveHolizon);

    //進行方向に見て右のレイ
    RayCastData lMoveRay;
    XMStoreFloat3(&lMoveRay.start, vPlayerPos_ + startVec[(int)DIRECTION::DIR_RIGHT]);
    XMStoreFloat3(&lMoveRay.dir, XMVector3Rotate(moveHolizon,XMQuaternionRotationNormal(-baseUpVec_,-(float)(M_PI/2.0f))));
   
    //進行方向に見て左のレイ
    RayCastData rMoveRay;
    XMStoreFloat3(&rMoveRay.start, vPlayerPos_ + startVec[(int)DIRECTION::DIR_LEFT]);
    XMStoreFloat3(&rMoveRay.dir, XMVector3Rotate(moveHolizon, XMQuaternionRotationNormal(-baseUpVec_,(float)(M_PI/2.0f))));
   
    XMStoreFloat3(&URay.start,vPlayerPos_+startVec[(int)DIRECTION::DIR_DOWN]);
    XMStoreFloat3(&URay.dir, startVec[3]);    

    float da = XMVectorGetX(XMVector3Length(moveHolizon));
    
    ModelManager::RayCast(stageNum_, fMoveRay);
    if (fMoveRay.dist<VectorLength(moveHolizon)*speed_)
    {
        
        vPlayerPos_ = XMLoadFloat3(&transform_.position_);
        moveDist = { 0,0,0 };

        //壁ズリベクトル = レイが当たったポリゴンの法線*進行方向ベクトルと法線の内積
        wallzuri = MoveVectorControl(fMoveRay, moveHolizon);
        SetStatus(ATC_DEFAULT);
        if (flyFlag_)
        {
            flyFlag_ = false;
        }
    }

    ModelManager::RayCast(stageNum_, lMoveRay);
    if(lMoveRay.dist<hitdist_)
    {
        moveDist = { 0,0,0 };
        wallzuri = MoveVectorControl(lMoveRay, moveHolizon);
        SetStatus(ATC_DEFAULT);
        if (flyFlag_)
        {
            flyFlag_ = false;
        }
    }

    ModelManager::RayCast(stageNum_, rMoveRay);
    if(rMoveRay.dist<hitdist_)
    {
        moveDist = { 0,0,0 };
        wallzuri = MoveVectorControl(rMoveRay, moveHolizon);
        SetStatus(ATC_DEFAULT);
        if (flyFlag_)
        {
            flyFlag_ = false;
        }
    }

    //上レイの距離(dist)がhitdist_以下になったらy軸の座標を戻す
    ModelManager::RayCast(stageNum_, URay);
    if (URay.dist<hitdist_)
    {
        moveDist = { 0,0,0 };
        wallzuri = MoveVectorControl(URay, moveHolizon);
        SetStatus(ATC_DEFAULT);
        if (flyFlag_)
        {
            flyFlag_ = false;
        }
    }


    moveVector = XMLoadFloat3(&moveDist)+ wallzuri;

    if (transform_.position_.x <= -areaLimit_.x)
    {
        transform_.position_.x = -areaLimit_.x;
    }
    if (transform_.position_.x >= areaLimit_.x)
    {
        transform_.position_.x = areaLimit_.x;
    }

    if (transform_.position_.y <= areaLimit_.w)
    {
        transform_.position_.y = 0;
    }
    if (transform_.position_.y >= areaLimit_.y)
    {
        transform_.position_.y = areaLimit_.y;
    }

    if (transform_.position_.z <= -areaLimit_.z)
    {
        transform_.position_.z = -areaLimit_.z;
    }
    if (transform_.position_.z >= areaLimit_.z)
    {
        transform_.position_.z = areaLimit_.z;
    }
    vPlayerPos_ = XMLoadFloat3(&transform_.position_);
    
    XMStoreFloat3(&transform_.position_, vPlayerPos_ + moveVector* speed_);
    //下レイの距離(dist)がhitdist_以下になったらy軸の座標を戻す
    XMStoreFloat3(&DRay.start, vPlayerPos_ + startVec[(int)DIRECTION::DIR_UP]);
    XMStoreFloat3(&DRay.dir, startVec[(int)DIRECTION::DIR_DOWN]);
    ModelManager::RayCast(stageNum_, DRay);
    if (DRay.dist<hitdist_)
    {
        float d = 1.0f+cos(DRay.angle);
        if (moveY <= 0)
        {
            moveY = RAY_OFFSET - DRay.dist;
            velocity_ = 0;

            if (flyFlag_)
            {
                flyFlag_ = false;
            }
        }   
        else if(DRay.dist<d)
        {
            moveY = RAY_OFFSET - DRay.dist;
            flyFlag_ = false;
        }
        if(!jumpFlag_)
        airFlag_ = false;
        
        jumpFlag_ = false;
        groundFlag_ =true;
    }
    else
    {
        groundFlag_ = false;
    }
    transform_.position_.y += moveY;

    float speed = VectorLength(XMLoadFloat3(&transform_.position_) - prevPositionVec);
    if (speed >= ATTACK_SPEED)
    {
        SetStatus(ATC_ATTACK);
    }
    else
        SetStatus(ATC_DEFAULT);

        
        prevPositionVec = XMLoadFloat3(&transform_.position_);
}

void Player::SetStatus(int type)
{
    if(status_!=type)
    status_ = type;
}

void Player::OccurParticle()
{
    EmitterData data;
    
    //攻撃状態ならパーティクルを発生させる
    if (status_ & ATC_ATTACK)
    {
        XMVECTOR pos = XMLoadFloat3(&transform_.position_);
        data.textureFileName = "Assets\\Effect01.png";
        XMFLOAT3 particlePos;
        XMStoreFloat3(&particlePos, pos + (XMVector3Normalize(vPlayerMove_)));
        data.position = particlePos;
        data.positionErr = XMFLOAT3(0.2f, 0, 0.2f);
        data.delay = 0;
        data.number = 30;
        data.lifTime = 50;
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
        data.deltaColor = XMFLOAT4(0, 0, 0, -0.08f);
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
        //プレイヤーが攻撃状態だったら
        if (status_ & ATC_ATTACK)
        {
            OccurParticle();
            flyFlag_ = false;
            vFlyMove_ = -vFlyMove_*0.8f;
            XMStoreFloat3(&transform_.position_ ,vPlayerPos_);
            godFlag_ = true;
            godTime_ = MAX_GODTIME;
        }
        else if(godFlag_==false)
        {
            playerLife_--;
            playerLife_ = max(0, playerLife_);
            flyFlag_ = false;
            flyTime_ = 1.0f;
            vFlyMove_ = XMVector3Normalize(pTarget->GetPosition()-transform_.position_)*(-1.5f);
            ImageManager::SetAlpha(life_[playerLife_], 0);
        }

        if(godFlag_==false)
        {
            godFlag_ = true;
            godTime_ = MAX_GODTIME;
        }
        Audio::Play(hAudio_);
    }

    if (pTarget->GetTag() == "EnemyBullet")
    {
        if (!(status_ & ATC_ATTACK))
        {
            if (godFlag_ == false)
            {
                playerLife_--;
                playerLife_ = max(0, playerLife_);
                godFlag_ = true;
                godTime_ = MAX_GODTIME;
                flyFlag_ = false;
                flyTime_ = 1.0f;
                vFlyMove_ = XMVector3Normalize(pTarget->GetPosition() - transform_.position_) * (-0.5f);
                ImageManager::SetAlpha(life_[playerLife_], 0);
            }
        }
    }

    if (pTarget->GetTag() == "Item")
    {
        pItemGetter_->ItemAttach((ItemBase*)pTarget);
    }

    if (playerLife_ <= 0)
    {
        DelCollider(*this);
        return;
    }
}

void Player::Aim(RayCastData* ray)
{
    //レイキャストの判定距離の上限
    ray->distLimit = ASSISTLIMIT;
    aimFlag_ = true;
    lockOn_ = false;
    float toEnemyDist = -1.0f;

    //当たる位置の計算
    XMFLOAT3 bonePos = ModelManager::GetBonePosition(hModel_, "shotPos");
    XMVECTOR vPlayerDir = XMVector3TransformCoord(vBaseTarget_, matCamY_ * matCamX_);
    XMVECTOR vPtrDir = vPlayerDir;
    ray->start = bonePos;
    XMStoreFloat3(&ray->dir, vPtrDir);

    //エイムアシスト範囲内かどうか判定
    pSetter_->GetEnemyList(&enemyList_);
    if (enemyList_.size() > 0)
    {
        Enemy* pEnemy = AimAssist(ray);
        //エネミーを捕捉しているならエネミーの情報を取得、ロックオンフラグを立てる
        if (pEnemy != nullptr)
        {
            //エネミーの座標取得
            XMFLOAT3 toEnemy = pEnemy->GetTransform().position_;
            //エネミーへのベクトル生成
            vPtrDir = toEnemy - bonePos;
            
            //ベクトルの長さ
            toEnemyDist = VectorLength(vPtrDir);

            //レイキャストの始点と方向を入力
                XMStoreFloat3(&ray->dir, vPtrDir);
                ModelManager::RayCast(*ray);
            if (ray->hit&&ray->hitModelList.begin()->hModel==pEnemy->GethModel())
            {
                lockOn_ = true;
            }

            enemyNumber_ = pEnemy->GetObjectID();
        }
         //捕捉していない場合ロックオンフラグを降ろす
         else
         {
             enemyNumber_ = -1;
             lockOn_ = false;
         }
    }
    
    {
        XMFLOAT3 dir;
        if (lockOn_ == false)
        {
            XMStoreFloat3(&dir, vPlayerDir);
            ray->Init(bonePos, dir, ASSISTLIMIT);
            
            ModelManager::RayCast(*ray);
        }

        else
        {
            XMStoreFloat3(&dir, vPtrDir);
            ray->Init(bonePos, dir, ASSISTLIMIT);
            ModelManager::RayCast(*ray);
        }
    }

    //当たった位置にマーカー表示
    if (ray->hit && !flyFlag_)
    {
        pPointer_->SetPosition(StoreFloat3(ray->hitPos));
        pPointer_->SetDraw(ray->hit);
        
        pPointerLine_->AddPosition(bonePos);
        pPointerLine_->AddPosition(pPointer_->GetPosition());
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

XMVECTOR Player::MoveVectorControl(const RayCastData& data, const XMVECTOR& vec)
{
    //進行方向のベクトル+進行方向のベクトルと法線ベクトルの内積*法線ベクトル
    XMVECTOR rubVec = vec + VectorDot(-vec, data.normal)* data.normal;
    XMVECTOR back = (XMLoadFloat3(&data.start) + (XMLoadFloat3(&data.dir) * 2)) - data.hitPos;
    XMStoreFloat3(&transform_.position_, vPlayerPos_ + -back);
    return rubVec;
}

bool Player::IsAssistRange(const RayCastData& ray, const XMFLOAT3& targetPos, float length)
{
    //自分からtargetPosまでのベクトル
    XMVECTOR targetVec = XMLoadFloat3(&targetPos) -
                         XMLoadFloat3(&transform_.position_); 
    
    //targetVecがlength以下だったら
    if (VectorLength(targetVec) < length)
    {
        targetVec = XMVector3Normalize(targetVec);
        XMVECTOR dirVec = XMVector3Normalize(XMLoadFloat3(&ray.dir));

        //targetVecとdirVecの内積を求める
        float angle = acosf(VectorDot(dirVec, targetVec));

        //angle(ラジアン)が範囲内の時カメラの回転速度を遅くする
        if (angle > -AIM_ASSIST_ANGLE && angle < AIM_ASSIST_ANGLE)
        {
            rotateSpeed_ = rotateSpeed_ * angle + LOW_CAMERA_ROTATE_SPEED;
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
    //エネミーリストが空だったらnullptrを返す
    if (enemyList_.empty())
        return nullptr;

    float minRange = 9999.0f;
    Enemy* pEnemy=nullptr;
    auto i = enemyList_.begin();

    //エネミーのリストからエイムアシストの範囲にいるかどうか調べる
    for (auto itr = enemyList_.begin(); itr != enemyList_.end(); itr++)
    {
        //範囲内だったら
        if (IsAssistRange(*ray, (*itr)->GetPosition(), ray->distLimit))
        {
            XMFLOAT3 targetPos = (*itr)->GetPosition();
            float range = VectorLength(targetPos - ray->start);
            if (range < minRange)
            {
                minRange = range;
                pEnemy = (*itr);
            }
        }
    }

    return pEnemy;
}