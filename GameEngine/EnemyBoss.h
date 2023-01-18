#pragma once
#include"Enemy.h"


class EnemyBoss : public Enemy
{
private:
    int hModel_;
    int shotTime_;
    float rpm_;


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
    void ChangeState(EnemyState<EnemyBoss>* state);
public:
    //コンストラクタ
    EnemyBoss(GameObject* parent);

    //デストラクタ
    ~EnemyBoss();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    void FixedUpdate() override;
    //描画
    void Draw() override;

    void Attack() override;

    //エネミーの動き
    //動く方向ベクトル
    void Shot();
    void HShot();

    //開放
    void Release() override;

    //衝突
    void OnCollision(GameObject* pTarget) override;
};

