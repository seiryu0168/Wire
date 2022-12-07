#pragma once
#include"Engine/GameObject.h"
#include"EnemyState.h"
#include"Player.h"
class Enemy : public GameObject
{
protected:
    EnemyState* pEnemyState_;

    bool     visibleFlag_;  //プレイヤーを見つけているかどうか
    bool     isTargetList_; //プレイヤーのターゲットリストに入ってるかどうか

    int      life_;         //敵の体力

    XMMATRIX matX_;         //X軸の回転行列
    XMMATRIX matY_;         //Y軸の回転行列
    XMVECTOR toPlayerVec_;  //プレイヤーに向かうベクトル
    XMVECTOR upVec_;        //上方向ベクトル(プレイヤーとの位置関係を求める時に使ったりする)
    XMVECTOR vPosition_;    //現在位置のベクトル
    Player* pPlayer_;      //プレイヤーのポインタ
    //コンストラクタ
    Enemy(GameObject* parent,std::string name);

    //デストラクタ
    ~Enemy();
public:
    XMVECTOR frontVec_;     //前方向ベクトル(このベクトルを基準にして視界が決まる)
    void ChangeState(EnemyState* state);
    bool IsVisible(XMVECTOR front,float angle,float range);
    virtual void Attack()=0;
    XMMATRIX GetMatrixX() { return matX_; }
    XMMATRIX GetMatrixY() { return matY_; }
    XMVECTOR GetToPlayerVec() { return toPlayerVec_; }
    XMVECTOR GetUpVec() { return upVec_; }
    Player* GetPlayerPointer() { return pPlayer_; }
    //isTargetListのフラグ取得
    bool GetIsList() { return isTargetList_; }

};

