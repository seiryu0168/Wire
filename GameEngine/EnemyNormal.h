#pragma once
#include"Enemy.h"

class EnemyNormal : public Enemy
{
private:
    int      hModel_;       //モデル番号

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
};