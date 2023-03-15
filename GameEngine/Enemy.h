#pragma once
#include"Engine/GameObject/GameObject.h"
#include"EnemyState.h"
#include"Engine/ResourceManager/Model.h"
#include"Player.h"


struct parameter
{
    bool     visibleFlag;  //プレイヤーを見つけているかどうか
    bool     isTargetList; //プレイヤーのターゲットリストに入ってるかどうか

    int      life;         //敵の体力
    int      hModel_;

    XMMATRIX matX;         //X軸の回転行列
    XMMATRIX matY;         //Y軸の回転行列
    XMVECTOR frontVec;     //前方向ベクトル(このベクトルを基準にして視界が決まる)
    XMVECTOR toPlayerVec;  //プレイヤーに向かうベクトル
    XMVECTOR upVec;        //上方向ベクトル(プレイヤーとの位置関係を求める時に使ったりする)
    XMVECTOR vPosition;    //現在位置のベクトル
    Player*  pPlayer;      //プレイヤーのポインタ
};

class Enemy : public GameObject
{
private:
    parameter enemyParameter_;
    //EnemyState* pEnemyState_;
public:
    //コンストラクタ
    Enemy(GameObject* parent,std::string name);

    virtual void Initialize() override {};

    virtual void Update() override {};

    virtual void Draw() override {};

    virtual void Release() override {};
    //デストラクタ
    ~Enemy();
    
    virtual void Attack() {};
    /// <summary>
/// エネミーの視界内にプレイヤーがいるかどうか
/// </summary>
/// <param name="visibleAngle">視角</param>
/// <param name="range">見える距離</param>
/// <returns></returns>
    bool IsVisible(float angle,float range);
    void TurnToPlayer(XMVECTOR vToPlayer);
    bool IsLockOned( Enemy* enemy);

    
   // void ChangeState(EnemyState* state);                                         //エネミーの状態変更
    //void SetviewRange(float range) { enemyParameter_.viewRange = range; }        //見える距離設定
    //void SetviewAngle(float angle) { enemyParameter_.viewAngle = angle; }        //視角設定
    void LoadModel(std::string fileName);                                          //モデルのロード
    void SetToPlayerVec(XMVECTOR vec) { enemyParameter_.toPlayerVec = vec; }     //プレイヤーに向かうベクトルの設定
    void SetPositionVec(XMVECTOR vec) { enemyParameter_.vPosition = vec; }       //ポジションベクトルの設定
    void SetFrontVec(XMVECTOR vec) { enemyParameter_.frontVec = vec; }           //前方向ベクトルの設定
    void SetMatrixX(XMMATRIX mat) { enemyParameter_.matX = mat; }                //X軸の回転行列
    void SetMatrixY(XMMATRIX mat) { enemyParameter_.matY = mat; }                //Y軸の回転行列
    void SetPlayerPointer(Player* pointer) { enemyParameter_.pPlayer = pointer; }//プレイヤーのポインター
    void SetLife(int life) { enemyParameter_.life = life; }                      //エネミーのライフ設定
    void DecreaseLife(int decRate) { enemyParameter_.life -= decRate; }          //ライフを減らす
    void IncreaseLife(int incRate) { enemyParameter_.life += incRate; }          //ライフを増やす
    void SetIsList(bool isList) { enemyParameter_.isTargetList = isList; }       //プレイヤーの認識リストに入ってるかどうかを設定
    void SethModel(int modelHandle) { enemyParameter_.hModel_ = modelHandle; }

    //float    GetViewAngle() { return enemyParameter_.viewAngle; }       //見える距離取得
    //float    GetViewRange() { return enemyParameter_.viewRange; }       //視角取得
    XMMATRIX GetMatrixX() { return enemyParameter_.matX; }              //X軸回転行列取得
    XMMATRIX GetMatrixY() { return enemyParameter_.matY; }              //Y軸回転行列取得
    XMVECTOR GetToPlayerVec() { return enemyParameter_.toPlayerVec; }   //プレイヤーに向かうベクトル取得
    XMVECTOR GetPositionVec() { return enemyParameter_.vPosition; }     //ポジションベクトル取得
    XMVECTOR GetUpVec() { return enemyParameter_.upVec; }               //上方向ベクトル取得
    XMVECTOR GetFrontVec() { return enemyParameter_.frontVec; }         //前方向ベクトル取得
    Player*  GetPlayerPointer() { return enemyParameter_.pPlayer; }     //プレイヤーのポインター取得
    bool     GetIsList() { return enemyParameter_.isTargetList; }       //エネミーがプレイヤーの認識リストに入ってるかどうか
    int      GetLife() { return enemyParameter_.life; }                 //エネミーのライフ取得
    bool     GetVisibleFrag() { return enemyParameter_.visibleFlag; }   //プレイヤーを認識してるかどうか
    int GethModel() { return enemyParameter_.hModel_; }

protected:
    //部品化 : エネミーの視界はクラスにしておいて、必要な関数を実装しておく
    class Sight
    {
    public:
            float angle_;
            float range_;
        Sight()
        {
            angle_ = (float)(M_PI / 2.0f);
            range_ = 50.0f;
        }

        void SetRange(float range)
        {
            assert(range >= 0.0f);
            range_ = range;
        }
        void SetAngle(float angle)
        {
            assert(angle > 0.0f && angle < M_PI * 2.0f);
            angle_ = angle;
        }
    };
    Sight sight;
};

