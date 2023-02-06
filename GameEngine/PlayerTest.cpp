#include "PlayerTest.h"
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
    static const float hitdist_ = 2.001f;
    static const int   MAX_LIFE = 10;
    static const float LIFE_OFFSET_X = -1800.0f;
    static const float LIFE_OFFSET_Y = -900.0f;
    static const float ACCEL_AOV = 70.0f;
    static const float NORMAL_AOV = 45.0f;
    static const float CAMERA_DIST = 1.5f;
}


PlayerTest::PlayerTest(GameObject* parent)
	:GameObject(parent,"PlayerTest")
{
}

PlayerTest::~PlayerTest()
{
}

void PlayerTest::Initialize()
{
}

void PlayerTest::Update()
{
    rotateSpeed_ = 4.0f;
    vPlayerPos_ = XMLoadFloat3(&transform_.position_);
    XMVECTOR vFly = XMVectorSet(0, 0, 0, 0);

    pPointer_->SetDraw(false);
    RayCastData ray;
    CheckTargetList();
    aimFlag_ = false;

    //�g���K�[�������ƈړ��ł���ǂɃ}�[�J�[���\�������
    if (Input::GetLTrigger())
    {
        ChangeState(StateAim::GetInstance());
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
            vFlyMove_ = XMVector3Normalize(ray.hitPos - vPlayerPos_) * maxSpeed_;
            SetStatus(pPointer_->GetObjectType());
        }
    }
    //�������ĂȂ�������W�����v
    else if (Input::IsPadButtonDown(XINPUT_GAMEPAD_A) && airFlag_ == false)
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
    float moveX = 0;
    float moveZ = 0;

    //�d�͉��Z
    vFly += XMVectorSet(0, velocity_, 0, 0);

    //���C���[�Ŕ��łȂ���Έړ��Ɨ������o����
    if (flyFlag_ == false)
    {
        moveX = Input::GetLStick_X();
        moveZ = Input::GetLStick_Y();
        if (airFlag_ == true && jumpFlag_ == false && groundFlag_ == false)
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
    if (abs(moveX) > 0 || abs(moveZ) > 0)
    {
        moveTime_ += 0.1f;
        moveTime_ = min(moveTime_, 1);
    }
    else
    {
        moveTime_ -= 0.05f;
        moveTime_ = max(moveTime_, 0);
    }

    //���C���[�Ŕ��ł���Ώ��X�ɉ������A���łȂ���Ώ��X�Ɍ���
    if (flyFlag_)
    {
        flyTime_ += 0.01f;
        flyTime_ = min(flyTime_, 1);
    }
    else
    {
        flyTime_ -= 0.01f;
        flyTime_ = max(flyTime_, 0);
    }
    pLine_->AddPosition(transform_.position_);

    //�s��ňړ��̃x�N�g�����J�����̌����ɕό`
    vMove = XMVector3TransformCoord(vMove, matCamX_);

    vPlayerMove_ = vMove;
    velocity_ = max(velocity_, -2);
    vPlayerMove_ += XMVectorLerp(XMVectorSet(0, 0, 0, 0), vFlyMove_, Easing::EaseOutQuad(flyTime_));
    vPlayerMove_ += vFly;
    CharactorControll(vPlayerMove_);
    XMStoreFloat3(&transform_.position_, vPlayerPos_ + vPlayerMove_);
    CameraMove(ray);


}

void PlayerTest::Draw()
{
}

void PlayerTest::SecondDraw()
{
}

void PlayerTest::CharactorControll(XMVECTOR& moveVector)
{
    RayCastData URay;
    RayCastData DRay;


    XMFLOAT3 moveDist;
    float moveY;

    XMStoreFloat3(&moveDist, moveVector);
    moveY = moveDist.y;
    moveDist.y = 0;                         //�x�N�g����y����0�ɂ���
    XMVECTOR moveHolizon = XMLoadFloat3(&moveDist);
    XMVECTOR startVec[5] = { 0 };
    startVec[0] = -XMVector3Normalize(moveHolizon);                                                //�i�s����
    startVec[1] = -XMVector3Rotate(-startVec[0], XMQuaternionRotationNormal(baseUpVec_, M_PI / 2));   //�i�s�����Ɍ��ĉE
    startVec[2] = -XMVector3Rotate(-startVec[0], XMQuaternionRotationNormal(baseUpVec_, -(M_PI / 2)));  //�i�s�����Ɍ��č�
    startVec[3] = baseUpVec_;                                                                      //��x�N�g��
    startVec[4] = -baseUpVec_;                                                                     //���x�N�g��
    XMVECTOR wallzuri = XMVectorSet(0, 0, 0, 0);

    //�i�s�����̃��C
    RayCastData fMoveRay;
    XMStoreFloat3(&fMoveRay.start, vPlayerPos_ + startVec[0]);
    XMStoreFloat3(&fMoveRay.dir, moveHolizon);
    ModelManager::RayCast(stageNum_, fMoveRay);

    //�i�s�����Ɍ��ĉE�̃��C
    RayCastData lMoveRay;
    XMStoreFloat3(&lMoveRay.start, vPlayerPos_ + startVec[1]);
    XMStoreFloat3(&lMoveRay.dir, XMVector3Rotate(moveHolizon, XMQuaternionRotationNormal(-baseUpVec_, -(M_PI / 2))));
    ModelManager::RayCast(stageNum_, lMoveRay);

    //�i�s�����Ɍ��č��̃��C
    RayCastData rMoveRay;
    XMStoreFloat3(&rMoveRay.start, vPlayerPos_ + startVec[2]);
    XMStoreFloat3(&rMoveRay.dir, XMVector3Rotate(moveHolizon, XMQuaternionRotationNormal(-baseUpVec_, (M_PI / 2))));
    ModelManager::RayCast(stageNum_, rMoveRay);

    XMStoreFloat3(&URay.start, vPlayerPos_ + startVec[4]);
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
        wallzuri = moveHolizon + (fMoveRay.normal * (1 - XMVectorGetX(XMVector3Dot(-moveHolizon, fMoveRay.normal))));
        XMVECTOR back = (XMLoadFloat3(&fMoveRay.start) + (XMLoadFloat3(&fMoveRay.dir) * 2)) - fMoveRay.hitPos;
        XMStoreFloat3(&transform_.position_, vPlayerPos_ + (-back));

        if (flyFlag_)
        {
            flyFlag_ = false;
        }
    }

    if (lMoveRay.dist < hitdist_)
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

    if (rMoveRay.dist < hitdist_)
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

void PlayerTest::CameraMove(RayCastData ray)
{
}

void PlayerTest::OnCollision(GameObject* pTarget)
{
    if (pTarget->GetTag() == "Enemy")
    {
        if (status_ & ATC_ATTACK)
        {
            OccurParticle();
            flyFlag_ = false;
            XMStoreFloat3(&transform_.position_, vPlayerPos_);
            vFlyMove_ = -vFlyMove_;
            godFlag_ = true;
            godTime_ = 30;
        }

        if (godFlag_ == false)
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
        InterSceneData::AddData("Result", nullptr, nullptr, nullptr, &result);
        ((SceneManager*)FindObject("SceneManager"))->ChangeScene((int)SCENE_ID::SCENE_ID_RESULT);
        return;
    }

    ImageManager::SetAlpha(life_[playerLife_ - 1], 0);
}

bool PlayerTest::IsAssistRange(XMVECTOR dirVec, XMFLOAT3 targetVec, float length)
{
    return false;
}

void PlayerTest::StateJump::Init(PlayerTest& player)
{
}

void PlayerTest::StateJump::Update(PlayerTest& player)
{
}

void PlayerTest::Release()
{
}

void PlayerTest::StateAim::Init(PlayerTest& player)
{
	player.aimTime_ = 0;
}

void PlayerTest::StateAim::Update(PlayerTest& player)
{

    RayCastData ray;
	if (Input::GetLTrigger())
	{
        player.aimTime_+=0.05f;
        player.aimTime_ = max(player.aimTime_, 1);
        //���C�L���X�g�̔��苗���̏��
        ray.distLimit = 100.0f;
        //������ʒu�̌v�Z
        XMVECTOR vPlayerDir = XMVector3TransformCoord(player.vBaseTarget_, player.matCamY_ * player.matCamX_);
        XMVECTOR vPtrDir = vPlayerDir;
        XMStoreFloat3(&ray.start, player.vPlayerPos_);
        XMStoreFloat3(&ray.dir, vPtrDir);

        //�G�C���A�V�X�g�͈͓����ǂ�������
        player.pSetter_->GetEnemyList(&player.enemyList_);
        if (player.enemyList_.size() > 0)
        {
            Enemy* pEnemy = player.AimAssist(&ray);
            if (pEnemy != nullptr)
            {
                vPtrDir = XMVector3TransformCoord(vPtrDir, player.LookAtMatrix(pEnemy->GetTransform().position_, vPtrDir));
                XMStoreFloat3(&ray.dir, vPtrDir);
            }
        }

        //���C�L���X�g�̎n�_�ƕ��������
        ModelManager::RayCast(ray);

        //���������ʒu�Ƀ}�[�J�[�\��
        if (ray.hit)
        {
            player.rotateSpeed_ = 2.0f;
            XMFLOAT3 pointerPos;
            XMStoreFloat3(&pointerPos, ray.hitPos);
            player.pPointer_->SetPointerPos(pointerPos);
            player.pPointer_->SetDraw(ray.hit);
        }
	}
    else
    {
        player.aimTime_ += -0.07f;
        player.aimTime_ = min(player.aimTime_, 0);
    }

    if (Input::GetRTrigger() && player.pPointer_->IsDraw())
    {
        if (ray.hit)
        {
            //groundFlag_ = false;
            //airFlag_ = false;
            //flyFlag_ = true;
            player.flyTime_ = 1;
            player.transform_.position_.y += 0.2f;
            player.velocity_ = 0;
            player.vFlyMove_ = XMVector3Normalize(ray.hitPos - player.vPlayerPos_) * player.maxSpeed_;
            player.SetStatus(player.pPointer_->GetObjectType());
        }
    }
}

void PlayerTest::ChangeState(PlayerState<PlayerTest>* state)
{
    if (state != pState_)
    {
        pState_ = state;
        pState_->Init(*this);
    }
}