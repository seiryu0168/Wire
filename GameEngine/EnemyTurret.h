#pragma once
#include "Enemy.h"
class EnemyTurret : public Enemy
{
private:
    int hModel_;
    int shotTime_;
    float rpm_;//連射速度 Rounds Per Minute

public:
    //コンストラクタ
    EnemyTurret(GameObject* parent);

    //デストラクタ
    ~EnemyTurret();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    void Attack() override;
    void FixedUpdate() override;

    void Shot();
    //描画
    void Draw() override;

    //解放
    void Release() override;

    void OnCollision(GameObject* pTarget) override;
};

