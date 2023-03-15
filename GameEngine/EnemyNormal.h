#pragma once
#include"Enemy.h"
#include"EnemyState.h"
class EnemyNormal : public Enemy
{
private:
    int      hModel_;       //モデル番号
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
    //コンストラクタ
    EnemyNormal(GameObject* parent);

    //デストラクタ
    ~EnemyNormal();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    void FixedUpdate() override;
    //描画
    void Draw() override;

    void Attack() override;

    //エネミーの動き
    void EnemyMove();

    //開放
    void Release() override;

    //衝突
    void OnCollision(GameObject* pTarget) override;

    void AdjustStartPos(XMFLOAT3& pos);
};