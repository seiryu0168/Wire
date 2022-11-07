#pragma once
#include"Enemy.h"
#include"Player.h"

class Enemy_normal : public Enemy
{
public:
    int hModel_;

    XMVECTOR moveVec_;
    XMMATRIX matX_;
    XMMATRIX matY_;

    Player* pPlayer_;
private:
    //コンストラクタ
    Enemy_normal(GameObject* parent);

    //デストラクタ
    ~Enemy_normal();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    void FixedUpdate() override;
    //描画
    void Draw() override;

    //
    void EnemyRotate(XMVECTOR toVec);

    //開放
    void Release() override;
};