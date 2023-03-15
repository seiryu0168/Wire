#pragma once
#include "Enemy.h"
#include"EnemyState.h"
class EnemyTurret : public Enemy
{
private:
    int hModel_;
    int shotTime_;
    int reLoadTime_;
    int shotCount_;
    float rpm_;//�A�ˑ��x Rounds Per Minute

    EnemyState<EnemyTurret>* pState_;

    class StateChase : public EnemyState<EnemyTurret>
    {
    private:
        bool perShot_;

    public:
        static StateChase* GetInstance()
        {
            static StateChase* instance = nullptr;
            if (instance == nullptr)
            {
                instance = new StateChase();
            }
            return instance;
        }
        void Init(EnemyTurret& enemy) override;
        void Update(EnemyTurret& enemy) override;
    };

    class StateSearch : public EnemyState<EnemyTurret>
    {
    public:
        static StateSearch* GetInstance()
        {
            static StateSearch* instance = nullptr;
            if (instance == nullptr)
            {
                instance = new StateSearch();
            }
            return instance;
        }
        void Init(EnemyTurret& enemy) override;
        void Update(EnemyTurret& enemy) override;
    };

    void ChangeSatate(EnemyState<EnemyTurret>* state);
public:
    //�R���X�g���N�^
    EnemyTurret(GameObject* parent);

    //�f�X�g���N�^
    ~EnemyTurret();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    void Attack() override;
    void FixedUpdate() override;

    void Shot(bool isShot);
    //�`��
    void Draw() override;

    //���
    void Release() override;

    void OnCollision(GameObject* pTarget) override;
    void AdjustStartPos(XMFLOAT3& pos);
};

