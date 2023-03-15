#pragma once
#include"Enemy.h"
#include"EnemyState.h"
class EnemyNormal : public Enemy
{
private:
    int      hModel_;       //���f���ԍ�
    EnemyState<EnemyNormal>* pState_;
    
    class StateChase : public EnemyState<EnemyNormal>
    {
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
        void Init(EnemyNormal& enemy) override;
        void Update(EnemyNormal& enemy) override;
    };

    class StateSearch : public EnemyState<EnemyNormal>
    {
    public:
        ~StateSearch();
        static StateSearch* GetInstance()
        {
            static StateSearch* instance = nullptr;
            if (instance == nullptr)
            {
                instance = new StateSearch();
            }
            return instance;
        }
        void Init(EnemyNormal& enemy) override;
        void Update(EnemyNormal& enemy) override;
    };

    XMVECTOR knockBackVec_;
    int knockBackTime_;
    void ChangeState(EnemyState<EnemyNormal>* state);
public:
    //�R���X�g���N�^
    EnemyNormal(GameObject* parent);

    //�f�X�g���N�^
    ~EnemyNormal();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    void FixedUpdate() override;
    //�`��
    void Draw() override;

    void Attack() override;

    //�G�l�~�[�̓���
    void EnemyMove();

    //�J��
    void Release() override;

    //�Փ�
    void OnCollision(GameObject* pTarget) override;

    void AdjustStartPos(XMFLOAT3& pos);
};