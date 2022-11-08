#pragma once
#include"Enemy.h"
#include"Player.h"

class Enemy_normal : public Enemy
{
private:
    int hModel_;

    XMMATRIX matX_;
    XMMATRIX matY_;
    

    XMVECTOR frontVec_;
    XMVECTOR vPosition_;
    Player* pPlayer_;
public:
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

    //エネミーが動く
    void EnemyMove(XMVECTOR toVec);

    bool IsVisible(XMVECTOR vFront, XMVECTOR vTarget,float angle1,float angle2);

    //開放
    void Release() override;
};