#include "Player.h"
#include"Engine/Input.h"
#include"Engine/Model.h"
#include"Engine/Camera.h"
#include"Engine/SceneManager.h"
#include"InterSceneData.h"
#include"EngineTime.h"
#include"Engine/ImageManager.h"
#include"ObjectSetter.h"
#include"EnemyNormal.h"
#include"Engine/Particle.h"
#include"Easing.h"
#include"Stage1.h"
#include"Pointer.h"
#include"Wire.h"
#include<list>
namespace
{
    static const std::vector<Enemy*> enemyList_;
    static const float hitdist_   =2.001f;
    static const int   MAX_LIFE   = 10;
    static const float LIFE_OFFSET_X = -1800.0f;
    static const float LIFE_OFFSET_Y = -900.0f;
    static const float ACCEL_AOV     = 70.0f;
    static const float NORMAL_AOV    = 45.0f;
    static const float CAMERA_DIST   = 1.5f;
}

//�R���X�g���N�^
Player::Player(GameObject* parent)
    :GameObject(parent, "Player"),
    baseUpVec_(XMVectorSet(0, 1, 0, 0)),
    playerLife_(MAX_LIFE),
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
    groundFlag_(true),
    godFlag_(false)
{
}

//�f�X�g���N�^
Player::~Player()
{
}

//������
void Player::Initialize()
{
    SetTag("Player");
    pSetter_ = (ObjectSetter*)FindObject("ObjectSetter");
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

    for (int i = 0; i < MAX_LIFE; i++)
    {
        int hPict_ = ImageManager::Load("Assets\\Life.png");
        ImageManager::SetImagePos(hPict_, { LIFE_OFFSET_X+100*i, LIFE_OFFSET_Y, 0 });
        ImageManager::SetImageSize(hPict_, { 0.1f,0.1f,1.0f });

        life_.push_back(hPict_);

    }
}

//�X�V
void Player::Update()
{
    if (godFlag_)
    {
        godTime_--;
        godTime_ = max(0, godTime_);
    }
    if(godTime_==0)
    {
        godFlag_ = false;
    }
    rotateSpeed_ = 4.0f;
    vPlayerPos_   = XMLoadFloat3(&transform_.position_);
    XMVECTOR vFly = XMVectorSet(0, 0, 0, 0);

    pPointer_->SetDraw(false);
    RayCastData ray;
    CheckTargetList();
    aimFlag_ = false;
    
    //�g���K�[�������ƈړ��ł���ǂɃ}�[�J�[���\�������
    if (Input::GetLTrigger())
    {
        //���C�L���X�g�̔��苗���̏��
        ray.distLimit = 100.0f;
        aimFlag_ = true;
        //������ʒu�̌v�Z
        XMVECTOR vPlayerDir = XMVector3TransformCoord(vBaseTarget_, matCamY_ * matCamX_);
        XMVECTOR vPtrDir = vPlayerDir;
        XMStoreFloat3(&ray.start, vPlayerPos_);
        XMStoreFloat3(&ray.dir, vPtrDir);

        //�G�C���A�V�X�g�͈͓����ǂ�������
        pSetter_->GetEnemyList(&enemyList_);
        if (enemyList_.size() > 0)
        {
            Enemy* pEnemy = AimAssist(&ray);
            if (pEnemy != nullptr)
            {
                vPtrDir = XMVector3TransformCoord(vPtrDir, LookAtMatrix(pEnemy->GetTransform().position_, vPtrDir));
                XMStoreFloat3(&ray.dir, vPtrDir);
            }
        }
        
        //���C�L���X�g�̎n�_�ƕ��������
        ModelManager::RayCast(ray);

        //���������ʒu�Ƀ}�[�J�[�\��
        if (ray.hit && !flyFlag_)
        {
            rotateSpeed_ = 2.0f;
            XMFLOAT3 pointerPos;
            XMStoreFloat3(&pointerPos, ray.hitPos);
            pPointer_->SetPointerPos(pointerPos);
            pPointer_->SetDraw(ray.hit);
        }
    }

    //���C���ǂȂǂɓ������Ă��炻�̕����Ɍ������x�N�g�������
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
    //�������ĂȂ�������W�����v
    else if(Input::IsPadButtonDown(XINPUT_GAMEPAD_A)&&airFlag_==false)
    {
        //���C���[�Ŕ��ł���jumpFlag_��false�ɂ��A�����łȂ����true
        jumpFlag_ = flyFlag_ == true ? false : true;
        //�W�����v����ƃ��C���[�A�N�V�����͒��f�����̂�flyFlag_��false
        flyFlag_ = false;
        //�󒆂ɂ���̂�airFlag_��true
        airFlag_ = true;
        groundFlag_ = false;
        //���������̑��x�ύX
        velocity_ = 2;
        vFlyMove_ *= 0.3f;
        transform_.position_.y += 0.2f;
    }

    //���X�e�B�b�N�̓��͒l������ϐ�
    float moveX=0;
    float moveZ=0;

    //�d�͉��Z
    vFly += XMVectorSet(0, velocity_, 0, 0);

    //���C���[�Ŕ��łȂ���Έړ��Ɨ������o����
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

    //L,R�X�e�B�b�N�ňړ�
    XMVECTOR vMove = XMVectorSet(moveX, 0, moveZ, 0);
    
    //�X�e�B�b�N���X���Ă�Ώ��X�ɉ������A�X���ĂȂ���Ώ��X�Ɍ���
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

    //���C���[�Ŕ��ł���Ώ��X�ɉ������A���łȂ���Ώ��X�Ɍ���
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
    
    //�s��ňړ��̃x�N�g�����J�����̌����ɕό`
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

//�`��
void Player::Draw()
{
    ModelManager::SetTransform(hModel_, transform_);
    ModelManager::Draw(hModel_); 
}

void Player::SecondDraw()
{
    pLine_->Draw(&transform_);
    pWire_->Draw(&transform_);
}

//�J��
void Player::Release()
{
}

void Player::CameraMove(RayCastData ray)
{
    
    //�Ə����߂Ă��鎞
    if (aimFlag_ == true)
    {
        aimTime_ += 0.05f;
        aimTime_ = min(aimTime_, 1);
    }
    else
    {
        aimTime_ += -0.07f;
        aimTime_ = max(aimTime_, 0.5);
        flyMove_ = { 0, 0, 0 };
    }

    //���C���[�Ŕ��ł��鎞
    if (flyFlag_ == true)
    {
        aimFlag_ = false;
        aimTime_ += -0.08f;
        aimTime_ = max(aimTime_, 0);
    }

    float cameraRate = max(NORMAL_AOV,flyTime_ * ACCEL_AOV);
    Camera::SetAOV((M_PI / 180.0f) *cameraRate );
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
    vNormalCam = XMVector3TransformCoord(vCamPos_,  matCamY_ * matCamX_)* CAMERA_DIST;
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
    moveDist.y = 0;                         //�x�N�g����y����0�ɂ���
    XMVECTOR moveHolizon = XMLoadFloat3(&moveDist);
    XMVECTOR startVec[5] = { 0 };
    startVec[0] = -XMVector3Normalize(moveHolizon);                                                //�i�s����
    startVec[1] = -XMVector3Rotate(-startVec[0],XMQuaternionRotationNormal(baseUpVec_, M_PI/2));   //�i�s�����Ɍ��ĉE
    startVec[2] = -XMVector3Rotate(-startVec[0], XMQuaternionRotationNormal(baseUpVec_, -(M_PI/2)));  //�i�s�����Ɍ��č�
    startVec[3] = baseUpVec_;                                                                      //��x�N�g��
    startVec[4] = -baseUpVec_;                                                                     //���x�N�g��
    XMVECTOR wallzuri = XMVectorSet(0, 0, 0, 0);
   
    //�i�s�����̃��C
    RayCastData fMoveRay;
    XMStoreFloat3(&fMoveRay.start, vPlayerPos_+startVec[0]);
    XMStoreFloat3(&fMoveRay.dir, moveHolizon);
    ModelManager::RayCast(stageNum_, fMoveRay);

    //�i�s�����Ɍ��ĉE�̃��C
    RayCastData lMoveRay;
    XMStoreFloat3(&lMoveRay.start, vPlayerPos_ + startVec[1]);
    XMStoreFloat3(&lMoveRay.dir, XMVector3Rotate(moveHolizon,XMQuaternionRotationNormal(-baseUpVec_,-(M_PI/2))));
    ModelManager::RayCast(stageNum_, lMoveRay);

    //�i�s�����Ɍ��č��̃��C
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

        //�ǃY���x�N�g�� = ���C�����������|���S���̖@��*�i�s�����x�N�g���Ɩ@���̓���
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

    //�ヌ�C�̋���(dist)��1�ȉ��ɂȂ�����y���̍��W��߂�
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

    //�����C�̋���(dist)��moveY�ȉ��ɂȂ�����y���̍��W��߂�
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
            godFlag_ = true;
            godTime_ = 30;
        }

        if(godFlag_==false)
        {
            playerLife_--;
            playerLife_ = max(0, playerLife_);
            godFlag_ = true;
            godTime_ = 30;
        }
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
                godTime_ = 30;
            }
        }
    }

    if (playerLife_ == 0)
    {
        bool result = false;
        InterSceneData::AddData("Result",nullptr,nullptr,nullptr,&result);
        ((SceneManager*)FindObject("SceneManager"))->ChangeScene((int)SCENE_ID::SCENE_ID_RESULT);
        return;
    }

    ImageManager::SetAlpha(life_[playerLife_ - 1], 0);
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
                         XMLoadFloat3(&transform_.position_);                       //��������targetPos�܂ł̃x�N�g��
    
    //targetVec��length�ȉ���������
    if (XMVectorGetX(XMVector3Length(targetVec)) < length)
    {
        targetVec = XMVector3Normalize(targetVec);
        dirVec = XMVector3Normalize(dirVec);
        float angle = XMVectorGetX(XMVector3AngleBetweenNormals(dirVec, targetVec));//targetVec��dirVec�̓��ς����߂�

        //angle(���W�A��)���}0.4�̎��J�����̉�]���x��x������
        if (angle > -0.4f && angle < 0.4f)
        {
            rotateSpeed_ = rotateSpeed_ * angle + 0.6f;
        }

        //angle(���W�A��)��lockOnAngleLimit_�ȓ��������烍�b�N�I��
        if (angle > -lockOnAngleLimit_ && angle < lockOnAngleLimit_)
        {
            return true;
        }
    }
    return false;
}

Enemy* Player::AimAssist(RayCastData* ray)
{
    if (enemyList_.empty())
        return nullptr;

    float minRange = 9999.0f;
    Enemy* pEnemy=nullptr;
    auto i = enemyList_.begin();
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