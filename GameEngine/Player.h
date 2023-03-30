#pragma once
#include "Engine/GameObject/GameObject.h"

#include"Engine/DirectX_11/Fbx.h"
#include"Engine/DirectX_11/LineParticle.h"


class Particle;
class Enemy;
class Wire;
class Pointer;
class ObjectSetter;
class PlayScreen;
class Player : public GameObject
{
    enum STATUS
    {
        STATE_GROUND=0,
        STATE_AIR,
        STATE_FLY,
    };


    char status_;

    const float gravity_;
    
    bool flyFlag_;
    bool airFlag_;
    bool aimFlag_;
    bool jumpFlag_;
    bool groundFlag_;
    bool godFlag_;
    bool lockOn_;

    int hModel_;
    int hAudio_;
    int hAudioShoot_;
    int playerLife_;
    int stageNum_;
    int godTime_;
    int enemyNumber_;
    std::vector<int> life_;

    float moveTime_;
    float aimTime_;
    float flyTime_;
    float velocity_;
    float rotateSpeed_;
    float maxSpeed_;
    float wireLength_;
    float angleY_;
    float angleX_;
    float lockOnAngleLimit_;

    XMVECTOR baseUpVec_;
    XMVECTOR vCamPos_;
    XMVECTOR vPlayerPos_;
    XMVECTOR vPlayerMove_;
    XMVECTOR vBaseTarget_;
    XMVECTOR vFlyMove_;
    XMVECTOR vBaseAim_;
    XMVECTOR cameraShake_;
    XMMATRIX matCamX_;
    XMMATRIX matCamY_;


    Particle*     pParticle_;
    LineParticle* pPointerLine_;
    Wire*         wire_;
    Pointer*      pPointer_;
    PlayScreen*   pScreen_;
    ObjectSetter* pSetter_;
    std::list<Enemy*> enemyList_;


public:
    //�R���X�g���N�^
    Player(GameObject* parent);

    //�f�X�g���N�^
    ~Player();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;

    void SecondDraw() override;

    //�L�������ǂ����蔲���Ȃ��悤�ɂ���
    void CharactorControll(XMVECTOR &moveVector);
    
    //�J�����̋����𐧌�
    void CameraMove(RayCastData ray);

    //�v���C���[�̃��C���[�A�N�V�������̃X�e�[�^�X��ݒ�
    void SetStatus(int type);
    
    //�Փ˔���
    void OnCollision(GameObject* pTarget) override;

    //�G�C��
    void Aim(RayCastData* ray);
    
    //�G�C���A�V�X�g�͈͓��ɂ��邩�ǂ���
    bool IsAssistRange(const RayCastData& ray,const XMFLOAT3& targetVec,float length=9999.0f);
    
    //�F�����X�g���ŃG�C���A�V�X�g�\�Ȃ���A�V�X�g����
    Enemy* AimAssist(RayCastData* rey);
 
    //�v���C���[�̃X�e�[�^�X�擾
    char GetSatatus() { return status_; }
    
    //�v���C���[�̈ړ��x�N�g���擾
    XMVECTOR GetPlayerMove() { return vPlayerMove_; }
    
    //�p�[�e�B�N��
    void OccurParticle();

    //�G���v���C���[�̔F�����X�g�ɓ����(�G�C���A�V�X�g�̑Ώۂ����炷����)
    void AddTargetList(Enemy* target);

    //�F�����X�g�ɂ��邩�ǂ����`�F�b�N
    void CheckTargetList();

    //�G���v���C���[�̔F�����X�g����O��
    void DeleteTargetList(Enemy* target);

    //�J��
    void Release() override;




/////////////////////////////////�Z�b�^�[�A�Q�b�^�[//////////////////////////////////
    XMVECTOR GetvBaseTarget() { return vBaseTarget_; }
    XMVECTOR GetvPlayerPos() { return vPlayerPos_; }
    XMMATRIX GetCameraMatrixX() { return matCamX_; }
    XMMATRIX GetCameraMatrixY() { return matCamY_; }
    XMMATRIX GetCameraMatrix() { return matCamY_*matCamX_; }
    Pointer* GetPointer() { return pPointer_; }
    std::list<Enemy*> GetEnemyList() { return enemyList_; }
    int GetLife() { return (int)playerLife_; }
    int GetTargetEnemyNum() { return enemyNumber_; }
    bool IsLockOn() { return lockOn_; }
    bool IsAim() { return aimFlag_; }
    bool IsFly() { return flyFlag_; }
    bool IsJump() { return jumpFlag_; }
    bool IsAir() { return airFlag_; }
    bool IsGround(){return groundFlag_;}

    void SetRotateSpeed(float rotateSpeed) { rotateSpeed_ = rotateSpeed; }

};

