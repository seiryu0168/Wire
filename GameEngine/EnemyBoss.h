#pragma once
#include"Enemy.h"


class EnemyBoss : public Enemy
{
private:
    bool perShot_;
    int hModelCore_;
    int hModelShield_;
    int shotTime_;
    int shotCount_;
    int reLoadTime_;
    float rpm_;

    const int RELOAD;


    EnemyState<EnemyBoss>* pState_;

    class StateChase : public EnemyState<EnemyBoss>
    {
    public:
        static StateChase* GetInstance()
        {
            static StateChase* instance = nullptr;
            if (instance == nullptr)
            {
                instance = new StateChase;
            }
            return instance;
        }
        void Init(EnemyBoss& enemy) override;
        void Update(EnemyBoss& enemy) override;
    };

    class StateSearch : public EnemyState<EnemyBoss>
    {
    public:
        static StateSearch* GetInstance()
        {
            static StateSearch* instance = nullptr;
            if (instance == nullptr)
            {
                instance = new StateSearch;
            }
            return instance;
        }
        void Init(EnemyBoss& enemy) override;
        void Update(EnemyBoss&enemy) override;
    };

    class StateSecondMode : public EnemyState<EnemyBoss>
    {
    private:
        int waitTime_;
        int hModel_;
    public:
        static StateSecondMode* GetInstance()
        {
            static StateSecondMode* instance = nullptr;
            if (instance == nullptr)
            {
                instance = new StateSecondMode;
            }
            return instance;
        }
        void Init(EnemyBoss& enemy) override;
        void Update(EnemyBoss& enemy) override;
    };


    void ChangeState(EnemyState<EnemyBoss>* state);
public:
    //�R���X�g���N�^
    EnemyBoss(GameObject* parent);

    //�f�X�g���N�^
    ~EnemyBoss();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    void FixedUpdate() override;
    //�`��
    void Draw() override;

    void Attack() override;

    //�G�l�~�[�̓���
    //���������x�N�g��
    void Shot(bool shot);
    void HShot(bool shot);

    //�J��
    void Release() override;

    //�Փ�
    void OnCollision(GameObject* pTarget) override;
};

