#pragma once
#include"Enemy.h"
#include"Player.h"

class Enemy_normal : public Enemy
{
private:
    int hModel_;

    XMMATRIX matX_;
    XMMATRIX matY_; 
    bool visibleFlag_;
    

    XMVECTOR frontVec_;
    XMVECTOR upVec_;
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

    //エネミーの動き
    //動く方向ベクトル
    void EnemyMove(XMVECTOR toVec);
    
    //視界内にプレイヤーがいるかどうか
    //エネミーの前方向ベクトル、ターゲットの座標、視野、感知距離
    bool IsVisible(XMVECTOR vFront, XMVECTOR vTarget,float visibleAngle,float range);

    //開放
    void Release() override;
};