#pragma once
#include"Enemy.h"


class EnemyBoss : public Enemy
{
    int hModel_;
    int shotTime_;
    float rpm_;
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
    void EnemyMove();
    void Shot();
    void HShot();

    //開放
    void Release() override;

    //衝突
    void OnCollision(GameObject* pTarget) override;
};

