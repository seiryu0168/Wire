#pragma once
#include"Enemy.h"
#include"Player.h"

class EnemyNormal : public Enemy
{
private:
    bool     visibleFlag_;  //プレイヤーを見つけているかどうか
    bool     isTargetList_; //プレイヤーのターゲットリストに入ってるかどうか
    int      hModel_;       //モデル番号
    int      life_;         //敵の体力

    XMMATRIX matX_;         //X軸の回転行列
    XMMATRIX matY_;         //Y軸の回転行列
    XMVECTOR toPlayerVec_;  //プレイヤーに向かうベクトル
    XMVECTOR frontVec_;     //前方向ベクトル(このベクトルを基準にして視界が決まる)
    XMVECTOR upVec_;        //上方向ベクトル(プレイヤーとの位置関係を求める時に使ったりする)
    XMVECTOR vPosition_;    //現在位置のベクトル
    Player*  pPlayer_;      //プレイヤーのポインタ

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

    //プレイヤーへ向かうベクトルの取得
    XMVECTOR GetToPlayerVector();

    //isTargetListのフラグ取得
    bool GetIsList() { return isTargetList_; }

    //開放
    void Release() override;

    //衝突
    void OnCollision(GameObject* pTarget) override;
};