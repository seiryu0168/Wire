#pragma once
#include"Engine/GameObject/GameObject.h"
#include"EnemyState.h"
#include"Engine/ResourceManager/Model.h"
#include"Player.h"


struct parameter
{
    bool     visibleFlag;  //�v���C���[�������Ă��邩�ǂ���
    bool     isTargetList; //�v���C���[�̃^�[�Q�b�g���X�g�ɓ����Ă邩�ǂ���

    int      life;         //�G�̗̑�
    int      hModel_;

    XMMATRIX matX;         //X���̉�]�s��
    XMMATRIX matY;         //Y���̉�]�s��
    XMVECTOR frontVec;     //�O�����x�N�g��(���̃x�N�g������ɂ��Ď��E�����܂�)
    XMVECTOR toPlayerVec;  //�v���C���[�Ɍ������x�N�g��
    XMVECTOR upVec;        //������x�N�g��(�v���C���[�Ƃ̈ʒu�֌W�����߂鎞�Ɏg�����肷��)
    XMVECTOR vPosition;    //���݈ʒu�̃x�N�g��
    Player*  pPlayer;      //�v���C���[�̃|�C���^
};

class Enemy : public GameObject
{
private:
    parameter enemyParameter_;
    //EnemyState* pEnemyState_;
public:
    //�R���X�g���N�^
    Enemy(GameObject* parent,std::string name);

    virtual void Initialize() override {};

    virtual void Update() override {};

    virtual void Draw() override {};

    virtual void Release() override {};
    //�f�X�g���N�^
    ~Enemy();
    
    virtual void Attack() {};
    /// <summary>
/// �G�l�~�[�̎��E���Ƀv���C���[�����邩�ǂ���
/// </summary>
/// <param name="visibleAngle">���p</param>
/// <param name="range">�����鋗��</param>
/// <returns></returns>
    bool IsVisible(float angle,float range);
    void TurnToPlayer(XMVECTOR vToPlayer);
    bool IsLockOned( Enemy* enemy);

    
   // void ChangeState(EnemyState* state);                                         //�G�l�~�[�̏�ԕύX
    //void SetviewRange(float range) { enemyParameter_.viewRange = range; }        //�����鋗���ݒ�
    //void SetviewAngle(float angle) { enemyParameter_.viewAngle = angle; }        //���p�ݒ�
    void LoadModel(std::string fileName);                                          //���f���̃��[�h
    void SetToPlayerVec(XMVECTOR vec) { enemyParameter_.toPlayerVec = vec; }     //�v���C���[�Ɍ������x�N�g���̐ݒ�
    void SetPositionVec(XMVECTOR vec) { enemyParameter_.vPosition = vec; }       //�|�W�V�����x�N�g���̐ݒ�
    void SetFrontVec(XMVECTOR vec) { enemyParameter_.frontVec = vec; }           //�O�����x�N�g���̐ݒ�
    void SetMatrixX(XMMATRIX mat) { enemyParameter_.matX = mat; }                //X���̉�]�s��
    void SetMatrixY(XMMATRIX mat) { enemyParameter_.matY = mat; }                //Y���̉�]�s��
    void SetPlayerPointer(Player* pointer) { enemyParameter_.pPlayer = pointer; }//�v���C���[�̃|�C���^�[
    void SetLife(int life) { enemyParameter_.life = life; }                      //�G�l�~�[�̃��C�t�ݒ�
    void DecreaseLife(int decRate) { enemyParameter_.life -= decRate; }          //���C�t�����炷
    void IncreaseLife(int incRate) { enemyParameter_.life += incRate; }          //���C�t�𑝂₷
    void SetIsList(bool isList) { enemyParameter_.isTargetList = isList; }       //�v���C���[�̔F�����X�g�ɓ����Ă邩�ǂ�����ݒ�
    void SethModel(int modelHandle) { enemyParameter_.hModel_ = modelHandle; }

    //float    GetViewAngle() { return enemyParameter_.viewAngle; }       //�����鋗���擾
    //float    GetViewRange() { return enemyParameter_.viewRange; }       //���p�擾
    XMMATRIX GetMatrixX() { return enemyParameter_.matX; }              //X����]�s��擾
    XMMATRIX GetMatrixY() { return enemyParameter_.matY; }              //Y����]�s��擾
    XMVECTOR GetToPlayerVec() { return enemyParameter_.toPlayerVec; }   //�v���C���[�Ɍ������x�N�g���擾
    XMVECTOR GetPositionVec() { return enemyParameter_.vPosition; }     //�|�W�V�����x�N�g���擾
    XMVECTOR GetUpVec() { return enemyParameter_.upVec; }               //������x�N�g���擾
    XMVECTOR GetFrontVec() { return enemyParameter_.frontVec; }         //�O�����x�N�g���擾
    Player*  GetPlayerPointer() { return enemyParameter_.pPlayer; }     //�v���C���[�̃|�C���^�[�擾
    bool     GetIsList() { return enemyParameter_.isTargetList; }       //�G�l�~�[���v���C���[�̔F�����X�g�ɓ����Ă邩�ǂ���
    int      GetLife() { return enemyParameter_.life; }                 //�G�l�~�[�̃��C�t�擾
    bool     GetVisibleFrag() { return enemyParameter_.visibleFlag; }   //�v���C���[��F�����Ă邩�ǂ���
    int GethModel() { return enemyParameter_.hModel_; }

protected:
    //���i�� : �G�l�~�[�̎��E�̓N���X�ɂ��Ă����āA�K�v�Ȋ֐����������Ă���
    class Sight
    {
    public:
            float angle_;
            float range_;
        Sight()
        {
            angle_ = (float)(M_PI / 2.0f);
            range_ = 50.0f;
        }

        void SetRange(float range)
        {
            assert(range >= 0.0f);
            range_ = range;
        }
        void SetAngle(float angle)
        {
            assert(angle > 0.0f && angle < M_PI * 2.0f);
            angle_ = angle;
        }
    };
    Sight sight;
};

