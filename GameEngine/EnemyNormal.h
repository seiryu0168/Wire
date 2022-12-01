#pragma once
#include"Enemy.h"
#include"Player.h"

class EnemyNormal : public Enemy
{
private:
    int hModel_;
    int life_;

    XMMATRIX matX_;
    XMMATRIX matY_; 
    bool visibleFlag_;

    XMVECTOR toPlayerVec_;
    XMVECTOR frontVec_;
    XMVECTOR upVec_;
    XMVECTOR vPosition_;
    Player* pPlayer_;

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

    //エネミーの動き
    //動く方向ベクトル
    void EnemyMove(XMVECTOR toVec);
    
    //視界内にプレイヤーがいるかどうか
    //エネミーの前方向ベクトル、ターゲットの座標、視野、感知距離
    bool IsVisible(XMVECTOR vFront, XMVECTOR vTarget,float visibleAngle,float range);

    XMVECTOR GetToPlayerVector();

    //開放
    void Release() override;

    //衝突
    void OnCollision(GameObject* pTarget) override;
};