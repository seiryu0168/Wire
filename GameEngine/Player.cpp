#include"Engine/DirectX_11/Input.h"
#include"Engine/ResourceManager/Model.h"
#include"Engine/GameObject/Camera.h"
#include"Engine/SceneManager.h"
#include"Engine/Collider/SphereCollider.h"
#include"EngineTime.h"
#include"Engine/ResourceManager/ImageManager.h"
#include"Engine/DirectX_11/Particle.h"
#include"PlayerBase.h"
#include"PlayScreen.h"
#include"Player.h"
#include"InterSceneData.h"
#include"ObjectSetter.h"
#include"EnemyNormal.h"
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
    static const float ANGLE_MAX     = 69.0f;
    static const float ANGLE_MIN     = -89.0f;
    static const float SHAKE_RATE = 1.5f;
}

//�R���X�g���N�^
Player::Player(GameObject* parent)
    :GameObject(parent, "Player"),
    status_(STATE_GROUND),
    baseUpVec_(XMVectorSet(0, 1, 0, 0)),
    playerLife_(MAX_LIFE-1),
    gravity_(-0.06f),
    hModel_(-1),
    hAudio_(-1),
    stageNum_(-1),
    vCamPos_(XMVectorSet(0, 10, -30, 0)),
    vPlayerPos_(XMVectorSet(0, 0, 0, 0)),
    vBaseTarget_(XMVectorSet(0, 0, 80, 0)),
    vBaseAim_(XMVectorSet(3, 2, -4, 0)),
    vFlyMove_(XMVectorSet(0,0,0,0)),
    vPlayerMove_(XMVectorSet(0,0,0,0)),
    cameraShake_(XMVectorZero()),
    matCamX_(XMMatrixIdentity()),
    matCamY_(XMMatrixIdentity()),
    moveTime_(0),
    aimTime_(0),
    flyTime_(0),
    godTime_(0),
    velocity_(0),
    rotateSpeed_(4.0f),
    maxSpeed_(1.5f),
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
    pLine_(nullptr),
    pPinterLine_(nullptr),
    pParticle_(nullptr),
    pPointer_(nullptr),
    pSetter_(nullptr),
    wire_(nullptr)
{
}

//�f�X�g���N�^
Player::~Player()
{
}

//������
void Player::Initialize()
{
    //�^�O�ݒ�
    SetTag("Player");
    //�Z�b�^�[�̃|�C���^�[�擾
    pSetter_ = (ObjectSetter*)FindObject("ObjectSetter");
    
    //���f�����[�h
    hModel_ = ModelManager::Load("Assets\\WireShooter_Maya.fbx");
    assert(hModel_ >= 0);

    //�p�[�e�B�N���I�u�W�F�N�g����
    pParticle_ = Instantiate<Particle>(GetParent());
    Instantiate<PlayerBase>(this);
    
    //���C���p�[�e�B�N������
    pLine_ = new LineParticle;
    pPinterLine_ = new LineParticle;
    //�p�����[�^�ݒ�
    pLine_->SetLineParameter(0.5f, 20,0.4f);
    pPinterLine_->SetLineParameter(0.1f, 2);
    //���C���p�[�e�B�N���p�摜�̃��[�h
    pLine_->Load("Assets\\Line.png");
    pPinterLine_->Load("Assets\\Effect01.png");


    SphereCollider* pCollider = new SphereCollider(XMFLOAT3(0,0,0),2);
    AddCollider(pCollider);
    stageNum_ = ((Stage1*)GetParent()->FindChild("Stage1"))->GetModelHandle();
    
    //�}�[�J�[�𐶐�
    Instantiate<Pointer>(GetParent());
    pPointer_ = (Pointer*)FindObject("Pointer");
    pPointer_->SetPosition({ 9999.0f,9999.0f,9999.0f });
    transform_.position_ = XMFLOAT3(0, 20,0);
   
    //�����ʒu�p�̃��C�L���X�g����
    RayCastData firstRay;
    firstRay.start = transform_.position_;
       
    XMStoreFloat3(&firstRay.dir, XMVectorSet(0, -1, 0, 0));
    ModelManager::RayCast(stageNum_, firstRay);
    ModelManager::SetModelNum(stageNum_);

    if (firstRay.hit)
    {
        transform_.position_.y -= firstRay.dist-transform_.scale_.y;
    }

    for (int i = 0; i < MAX_LIFE; i++)
    {
        int hPict_ = ImageManager::Load("Assets\\LifeImage.png");
        ImageManager::SetImagePos(hPict_, { LIFE_OFFSET_X+100*i, LIFE_OFFSET_Y, 0 });
        ImageManager::SetImageSize(hPict_, { 0.2f,0.2f,1.0f });

        life_.push_back(hPict_);
    }
    pScreen_ = new PlayScreen;
    wire_ = new Wire;
}

//�X�V
void Player::Update()
{
    //���G���Ԃ̏���
    if (godFlag_)
    {
        cameraShake_ = XMVectorSet((float)(rand() % 10)/10, (float)(rand() % 10) / 10, 0, 0)*SHAKE_RATE;
        godTime_--;
        godTime_ = max(0, godTime_);
    }
    if(godTime_==0)
    {
        cameraShake_ = XMVectorSet(0, 0, 0, 0);
        godFlag_ = false;
    }

    //��]���x���|�W�V�����̎擾���̏���
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
        Aim(&ray);
    }
    else
    {
        enemyNumber_ = -1;
        lockOn_ = false;
    }

    //���C���ǂȂǂɓ������Ă��炻�̕����Ɍ������x�N�g�������
    if (Input::GetRTriggerDown() && pPointer_->IsDraw())
    {
        if (ray.hit)
        {
            groundFlag_ = false;
            airFlag_ = false;
            flyFlag_ = true;
            flyTime_ = 1;
            transform_.position_.y += 0.2f;
            velocity_ = 0;
            //vFlyMove_ = XMVector3Normalize(ray.hitPos - vPlayerPos_)* maxSpeed_;
            wire_->ShotWire(vPlayerPos_, ray.hitPos);
            
        }
        
    }
    wire_->Update();
    if (wire_->GetWireState()==WIRE_STATE::EXTEND)
    {
        vFlyMove_ = wire_->GetWireVec() * maxSpeed_;
        //if(lockOn_)
        SetStatus(ATC_ATTACK);
        
        pPointer_->SetPosition({ 9999.0f,9999.0f,9999.0f });
    }
    else if (wire_->GetWireState() == WIRE_STATE::STRETCH)
    {
        return;
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
        if (Input::IsKey(DIK_W))
        {
            moveZ = 1;
        }

        //�󒆂ɂ��āA�W�����v���ĂȂ���Ԃ̎��ړ��𐧌�����
        if (airFlag_ == true && jumpFlag_ == false&& groundFlag_==false)
        {
            moveX *= 0.3f;
            moveZ *= 0.3f;
        }

        //�n�ʂɂ��Ă鎞������
        if (groundFlag_ != true)
        {
            velocity_ += gravity_;
        }
    }
    pScreen_->Update({ moveX,moveZ });

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

    //�ړ�
    vPlayerMove_  = vMove;
    velocity_     = max(velocity_, -2);
    vPlayerMove_ += XMVectorLerp(XMVectorSet(0, 0, 0, 0), vFlyMove_, Easing::EaseOutQuad(flyTime_));
    vPlayerMove_ += vFly;
    CharactorControll(vPlayerMove_);
    XMStoreFloat3(&transform_.position_, vPlayerPos_+vPlayerMove_);
    CameraMove(ray);
    transform_.rotate_.y = angleY_;
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
    wire_->Draw(transform_);
    if (pPointer_->IsDraw())
        pPinterLine_->Draw(&transform_);
}

//�J��
void Player::Release()
{
    SAFE_DELETE(wire_);
    SAFE_RELEASE(pPointer_);
    SAFE_DELETE(pScreen_);  
    SAFE_RELEASE(pParticle_);
    SAFE_RELEASE(pLine_);
    SAFE_RELEASE(pPinterLine_);
}

void Player::CameraMove(RayCastData ray)
{
    
    //�Ə����߂Ă��鎞
    if (aimFlag_ == true)
    {
        aimTime_ += 0.05f;
        aimTime_ = (float)min(aimTime_, 1);
    }
    else
    {
        aimTime_ += -0.07f;
        aimTime_ = (float)max(aimTime_, 0.5);
    }

    //���C���[�Ŕ��ł��鎞
    if (flyFlag_ == true)
    {
        aimFlag_ = false;
        aimTime_ += -0.08f;
        aimTime_ = (float)max(aimTime_, 0);
    }

    float cameraRate = max(NORMAL_AOV,flyTime_ * ACCEL_AOV);
    Camera::SetAOV((float)(M_PI / 180.0) *cameraRate );
    angleX_ += -Input::GetRStick_Y() * rotateSpeed_;
    angleY_ += Input::GetRStick_X() * rotateSpeed_;

    vPlayerPos_ = XMLoadFloat3(&transform_.position_);
    XMVECTOR vMoveCam;
    XMVECTOR vTarCam;
    XMVECTOR vNormalCam;
    XMVECTOR vAimCam;

    if (angleX_ <= -90)
    {
        angleX_ = ANGLE_MIN;
    }
    if (angleX_ >= 70)
    {
        angleX_ = ANGLE_MAX;
    }


    matCamY_   = XMMatrixRotationX(angleX_ * (float)(M_PI / 180.0));
    matCamX_   = XMMatrixRotationY(angleY_ * (float)(M_PI / 180.0));
    vNormalCam = XMVector3TransformCoord(vCamPos_,  matCamY_ * matCamX_)* CAMERA_DIST;
    vAimCam    = XMVector3TransformCoord(vBaseAim_, matCamY_ * matCamX_);
    vTarCam    = vPlayerPos_+XMVector3TransformCoord(vBaseTarget_, matCamY_ * matCamX_)+cameraShake_;
    
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
    startVec[1] = -XMVector3Rotate(-startVec[0],XMQuaternionRotationNormal(baseUpVec_, (float)(M_PI/2.0f)));   //�i�s�����Ɍ��ĉE
    startVec[2] = -XMVector3Rotate(-startVec[0], XMQuaternionRotationNormal(baseUpVec_, -(float)(M_PI/2.0f)));  //�i�s�����Ɍ��č�
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
    XMStoreFloat3(&lMoveRay.dir, XMVector3Rotate(moveHolizon,XMQuaternionRotationNormal(-baseUpVec_,-(float)(M_PI/2.0f))));
    ModelManager::RayCast(stageNum_, lMoveRay);

    //�i�s�����Ɍ��č��̃��C
    RayCastData rMoveRay;
    XMStoreFloat3(&rMoveRay.start, vPlayerPos_ + startVec[2]);
    XMStoreFloat3(&rMoveRay.dir, XMVector3Rotate(moveHolizon, XMQuaternionRotationNormal(-baseUpVec_,(float)(M_PI/2.0f))));
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
        SetStatus(ATC_DEFAULT);
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
        SetStatus(ATC_DEFAULT);
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
        SetStatus(ATC_DEFAULT);
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
        SetStatus(ATC_DEFAULT);
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
            SetStatus(ATC_DEFAULT);
        }
    }
    else
        groundFlag_ = false;

    if (transform_.position_.x <= -250.0f)
    {
        transform_.position_.x = -250.0f;
    }
    if (transform_.position_.x >= 250.0f)
    {
        transform_.position_.x = 250.0f;
    }

    if (transform_.position_.y <= -250.0f)
    {
        transform_.position_.y = 5.0f;
    }
    if (transform_.position_.y >= 250.0f)
    {
        transform_.position_.y = 250.0f;
    }

    if (transform_.position_.z <= -250.0f)
    {
        transform_.position_.z = -250.0f;
    }
    if (transform_.position_.z >= 250.0f)
    {
        transform_.position_.z = 250.0f;
    }

    vPlayerPos_ = XMLoadFloat3(&transform_.position_);
    moveDist.y = moveY;
    moveVector = XMLoadFloat3(&moveDist);
    
    moveVector += wallzuri;
}

void Player::SetStatus(int type)
{
    if(status_!=type)
    status_ = type;
}

void Player::OccurParticle()
{
    EmitterData data;
    
    //�U����ԂȂ�p�[�e�B�N���𔭐�������
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
        //�v���C���[���U����Ԃ�������
        if (status_ & ATC_ATTACK)
        {
            OccurParticle();
            flyFlag_ = false;
            vFlyMove_ = -vFlyMove_;
            XMStoreFloat3(&transform_.position_ ,vPlayerPos_);
            godFlag_ = true;
            godTime_ = 30;
        }
        else if(godFlag_==false)
        {
            ImageManager::SetAlpha(life_[playerLife_], 0);
            playerLife_--;
            playerLife_ = max(0, playerLife_);
        }

        if(godFlag_==false)
        {
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
                ImageManager::SetAlpha(life_[playerLife_], 0);
                playerLife_--;
                playerLife_ = max(0, playerLife_);
                godFlag_ = true;
                godTime_ = 30;

            }
        }
    }

    if (playerLife_ < 0)
    {
        bool result = false;
        //InterSceneData::AddData("Result",nullptr,nullptr,nullptr,&result);
        DelCollider(*this);
        //ImageManager::SetAlpha(life_[playerLife_], 0);
        return;
    }

    //ImageManager::SetAlpha(life_[playerLife_], 0);
}

void Player::Aim(RayCastData* ray)
{
    //���C�L���X�g�̔��苗���̏��
    ray->distLimit = 100.0f;
    aimFlag_ = true;
    //������ʒu�̌v�Z
    XMFLOAT3 bonePos = ModelManager::GetBonePosition(hModel_, "shotPos");
    XMVECTOR vPlayerDir = XMVector3TransformCoord(vBaseTarget_, matCamY_ * matCamX_);
    XMVECTOR vPtrDir = vPlayerDir;
    ray->start = bonePos;
    XMStoreFloat3(&ray->dir, vPtrDir);

    //�G�C���A�V�X�g�͈͓����ǂ�������
    pSetter_->GetEnemyList(&enemyList_);
    if (enemyList_.size() > 0)
    {
        Enemy* pEnemy = AimAssist(ray);
        if (pEnemy != nullptr)
        {
            XMFLOAT3 toEnemy = pEnemy->GetTransform().position_;
            vPtrDir = XMLoadFloat3(&toEnemy) - XMLoadFloat3(&bonePos);
            XMStoreFloat3(&ray->dir, vPtrDir);
            lockOn_ = true;
            enemyNumber_ = pEnemy->GetObjectID();
        }
        else
            enemyNumber_ = -1;

    }

    //���C�L���X�g�̎n�_�ƕ��������
    ModelManager::RayCast(*ray);

    //���������ʒu�Ƀ}�[�J�[�\��
    if (ray->hit && !flyFlag_)
    {
        rotateSpeed_ = 2.0f;
        XMFLOAT3 pointerPos;
        XMStoreFloat3(&pointerPos, ray->hitPos);
        pPointer_->SetPointerPos(pointerPos);
        pPointer_->SetDraw(ray->hit);
        
        pPinterLine_->AddPosition(bonePos);
        pPinterLine_->AddPosition(pPointer_->GetPosition());
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

bool Player::IsAssistRange(const RayCastData& ray, const XMFLOAT3& targetPos, float length)
{
    
    //��������targetPos�܂ł̃x�N�g��
    XMVECTOR targetVec = XMLoadFloat3(&targetPos) -
                         XMLoadFloat3(&transform_.position_); 
    
    //targetVec��length�ȉ���������
    if (XMVectorGetX(XMVector3Length(targetVec)) < length)
    {
        targetVec = XMVector3Normalize(targetVec);
        XMVECTOR dirVec = XMVector3Normalize(XMLoadFloat3(&ray.dir));

        //targetVec��dirVec�̓��ς����߂�
        float angle = XMVectorGetX(XMVector3AngleBetweenNormals(dirVec, targetVec));

        //angle(���W�A��)���}0.4�̎��J�����̉�]���x��x������
        if (angle > -0.4f && angle < 0.4f)
        {
            rotateSpeed_ = rotateSpeed_ * angle + 0.55f;
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
        if (IsAssistRange(*ray, (*itr)->GetPosition(), ray->distLimit))
        {
            XMFLOAT3 targetPos = (*itr)->GetPosition();
            float range = XMVectorGetX(XMVector3Length(XMLoadFloat3(&targetPos) - XMLoadFloat3(&ray->start)));
            if (range < minRange)
            {
                minRange = range;
                pEnemy = (*itr);
            }
        }
    }

    return pEnemy;
}