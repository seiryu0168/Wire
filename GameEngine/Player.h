#pragma once
#include "Engine/GameObject/GameObject.h"
#include"Engine/DirectX_11/Fbx.h"
#include"Engine/DirectX_11/LineParticle.h"


class Particle;
class Enemy;
class Wire;
class Pointer;
class PlayerStatus;
class ObjectSetter;
class Player : public GameObject
{
    //enum DIR_NAME
    //{
    //    DIR_FRONT=0,
    //    DIR_BACK,
    //    DIR_LEFT,
    //    DIR_RIGHT,
    //    DIR_UP,
    //    DIR_DOWN
    //};
    enum STATUS
    {
        STATE_GROUND=0,
        STATE_AIR,
        STATE_FLY,
    };

    XMVECTOR baseUpVec_;
    XMVECTOR rayDir_[6];

    char status_;

    const float gravity_;
    
    bool flyFlag_;
    bool airFlag_;
    bool aimFlag_;
    bool jumpFlag_;
    bool groundFlag_;
    bool godFlag_;

    int hModel_;
    int hModel_Handle_;
    int hAudio_;
    int playerLife_;
    int stageNum_;
    int godTime_;
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


    XMVECTOR vCamPos_;
    XMVECTOR vPlayerPos_;
    XMVECTOR vPlayerMove_;
    XMVECTOR vBaseTarget_;
    XMVECTOR vFlyMove_;
    XMVECTOR vBaseAim_;
    XMFLOAT3 flyMove_;
    XMMATRIX matCamX_;
    XMMATRIX matCamY_;



    PlayerStatus* PlayerState_;
    Particle*     pParticle_;
    LineParticle* pLine_;
    LineParticle* pWire_;
    Pointer*      pPointer_;
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

    void FixedUpdate() override;
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

    void Aim(RayCastData* ray);
    
    //�G�C���A�V�X�g�͈͓��ɂ��邩�ǂ���
    bool IsAssistRange(XMVECTOR dirVec,XMFLOAT3 targetVec,float length=9999.0f);
    
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

    void SetRotateSpeed(float rotateSpeed) { rotateSpeed_ = rotateSpeed; }

};

