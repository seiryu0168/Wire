#pragma once
#include "Enemy.h"
class EnemyTurret : public Enemy
{
private:
    int hModel_;

    float targetingRange_;
    float viewangle_;

public:
    //コンストラクタ
    EnemyTurret(GameObject* parent);

    //デストラクタ
    ~EnemyTurret();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    void FixedUpdate() override;

    //描画
    void Draw() override;

    //解放
    void Release() override;
};

