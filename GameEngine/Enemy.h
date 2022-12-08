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
    void ChangeState(EnemyState* state);
    void SetviewRange(float range) { enemyParameter_.viewRange = range; }
    void SetviewAngle(float angle) { enemyParameter_.viewAngle = angle; }
    void SetToPlayerVec(XMVECTOR vec) { enemyParameter_.toPlayerVec = vec; }
    void SetPositionVec(XMVECTOR vec) { enemyParameter_.vPosition = vec; }
    void SetFrontVec(XMVECTOR vec) { enemyParameter_.frontVec = vec; }
    void SetMatrixX(XMMATRIX mat) { enemyParameter_.matX = mat; }
    void SetMatrixY(XMMATRIX mat) { enemyParameter_.matY = mat; }
    void SetPlayerPointer(Player* pointer) { enemyParameter_.pPlayer = pointer; }
    void SetLife(int life) { enemyParameter_.life = life; }
    void DecreaseLife(int decRate) { enemyParameter_.life -= decRate; }
    void IncreaseLife(int incRate) { enemyParameter_.life += incRate; }

    float GetViewAngle() { return enemyParameter_.viewAngle; }
    float GetViewRange() { return enemyParameter_.viewRange; }
    XMMATRIX GetMatrixX() { return enemyParameter_.matX; }
    XMMATRIX GetMatrixY() { return enemyParameter_.matY; }
    XMVECTOR GetToPlayerVec() { return enemyParameter_.toPlayerVec; }
    XMVECTOR GetPositionVec() { return enemyParameter_.vPosition; }
    XMVECTOR GetUpVec() { return enemyParameter_.upVec; }
    XMVECTOR GetFrontVec() { return enemyParameter_.frontVec; }
    Player* GetPlayerPointer() { return enemyParameter_.pPlayer; }
    bool GetIsList() { return enemyParameter_.isTargetList; }
    int GetLife() { return enemyParameter_.life; }
    EnemyState* GetEnemyState() { return pEnemyState_; }

};

