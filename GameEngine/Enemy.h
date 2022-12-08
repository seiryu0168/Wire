#pragma once
#include"Engine/GameObject.h"
#include"EnemyState.h"
#include"Player.h"
struct parameter
{
    bool     visibleFlag;  //プレイヤーを見つけているかどうか
    bool     isTargetList; //プレイヤーのターゲットリストに入ってるかどうか

    int      life;         //敵の体力

    float    viewRange;
    float    viewAngle;

    XMMATRIX matX;         //X軸の回転行列
    XMMATRIX matY;         //Y軸の回転行列
    XMVECTOR frontVec;     //前方向ベクトル(このベクトルを基準にして視界が決まる)
    XMVECTOR toPlayerVec;  //プレイヤーに向かうベクトル
    XMVECTOR upVec;        //上方向ベクトル(プレイヤーとの位置関係を求める時に使ったりする)
    XMVECTOR vPosition;    //現在位置のベクトル
    Player* pPlayer;      //プレイヤーのポインタ
};

class Enemy : public GameObject
{
private:
    parameter enemyParameter_;
    EnemyState* pEnemyState_;
public:
    //コンストラクタ
    Enemy(GameObject* parent,std::string name);

    //デストラクタ
    ~Enemy();
    
    virtual void Attack()=0;
    
    bool IsVisible(float angle,float range);//指定した視野内にプレイヤーがいるかどうか : 視角、見える距離
    
    void ChangeState(EnemyState* state);                                         //エネミーの状態変更
    void SetviewRange(float range) { enemyParameter_.viewRange = range; }        //見える距離設定
    void SetviewAngle(float angle) { enemyParameter_.viewAngle = angle; }        //視角設定
    void SetToPlayerVec(XMVECTOR vec) { enemyParameter_.toPlayerVec = vec; }     //プレイヤーに向かうベクトルの設定
    void SetPositionVec(XMVECTOR vec) { enemyParameter_.vPosition = vec; }       //ポジションベクトルの設定
    void SetFrontVec(XMVECTOR vec) { enemyParameter_.frontVec = vec; }           //前方向ベクトルの設定
    void SetMatrixX(XMMATRIX mat) { enemyParameter_.matX = mat; }                //X軸の回転行列
    void SetMatrixY(XMMATRIX mat) { enemyParameter_.matY = mat; }                //Y軸の回転行列
    void SetPlayerPointer(Player* pointer) { enemyParameter_.pPlayer = pointer; }//プレイヤーのポインター
    void SetLife(int life) { enemyParameter_.life = life; }                      //エネミーのライフ設定
    void DecreaseLife(int decRate) { enemyParameter_.life -= decRate; }          //ライフを減らす
    void IncreaseLife(int incRate) { enemyParameter_.life += incRate; }          //ライフを増やす

    float    GetViewAngle() { return enemyParameter_.viewAngle; }       //見える距離取得
    float    GetViewRange() { return enemyParameter_.viewRange; }       //視角取得
    XMMATRIX GetMatrixX() { return enemyParameter_.matX; }              //X軸回転行列取得
    XMMATRIX GetMatrixY() { return enemyParameter_.matY; }              //Y軸回転行列取得
    XMVECTOR GetToPlayerVec() { return enemyParameter_.toPlayerVec; }   //プレイヤーに向かうベクトル取得
    XMVECTOR GetPositionVec() { return enemyParameter_.vPosition; }     //ポジションベクトル取得
    XMVECTOR GetUpVec() { return enemyParameter_.upVec; }               //上方向ベクトル取得
    XMVECTOR GetFrontVec() { return enemyParameter_.frontVec; }         //前方向ベクトル取得
    Player*  GetPlayerPointer() { return enemyParameter_.pPlayer; }     //プレイヤーのポインター取得
    bool     GetIsList() { return enemyParameter_.isTargetList; }       //エネミーがプレイヤーの認識リストに入ってるかどうか
    int      GetLife() { return enemyParameter_.life; }                 //エネミーのライフ取得
    EnemyState* GetEnemyState() { return pEnemyState_; }                //エネミーの状態取得

};

