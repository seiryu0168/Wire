#pragma once
#include "Engine/GameObject/GameObject.h"

#include"Engine/DirectX_11/Fbx.h"
#include"Engine/DirectX_11/LineParticle.h"



class Particle;
class Enemy;
class Wire;
class Pointer;
class ObjectSetter;
class PlayScreen;
class ModelComponent;
class ItemGetter;
class Player : public GameObject
{

    enum class DIRECTION
    {
        DIR_FRONT=0,
        DIR_RIGHT,
        DIR_LEFT,
        DIR_UP,
        DIR_DOWN,
        DIR_MAX,
    };

    enum class PLAYER_STATUS
    {
        LIVING=0,
        DEATH,
    };

    PLAYER_STATUS playerStatus;

    char status_;
    char prevHitBit_;

    const float gravity_;
    
    bool flyFlag_;
    bool airFlag_;
    bool aimFlag_;
    bool jumpFlag_;
    bool groundFlag_;
    bool godFlag_;
    bool lockOn_;


    int hModel_;
    int hAudio_;
    int hAudioShoot_;
    int playerLife_;
    int stageNum_;
    int godTime_;
    int enemyNumber_;
    std::vector<int> life_;

    float moveTime_;
    float aimTime_;
    float flyTime_;
    float velocity_;
    float rotateSpeed_;
    float speed_;
    float maxSpeed_;
    float wireLength_;
    float angleY_;
    float angleX_;
    float lockOnAngleLimit_;
    
    XMFLOAT4 areaLimit_;
    XMVECTOR prevPositionVec;
    XMVECTOR baseUpVec_;
    XMVECTOR vCamPos_;
    XMVECTOR vPlayerPos_;
    XMVECTOR vPlayerMove_;
    XMVECTOR vBaseTarget_;
    XMVECTOR vFlyMove_;
    XMVECTOR vBaseAim_;
    XMVECTOR cameraShake_;
    XMMATRIX matCamX_;
    XMMATRIX matCamY_;


    Particle*     pParticle_;
    LineParticle* pPointerLine_;
    Wire*         wire_;
    Pointer*      pPointer_;
    PlayScreen*   pScreen_;
    ObjectSetter* pSetter_;
    //ModelComponent* modelComp_;
    ItemGetter* pItemGetter_;
    std::list<Enemy*> enemyList_;


public:
    //コンストラクタ
    Player(GameObject* parent);

    //デストラクタ
    ~Player();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    void SecondDraw() override;
    void ThirdDraw() override;

    void LivingUpdate();
    void DeathUpdate();

    //キャラが壁をすり抜けないようにする
    void CharactorControll(XMVECTOR &moveVector);

    XMVECTOR MoveVectorControl(const RayCastData& data, const XMVECTOR& vec);
    
    //カメラの挙動を制御
    void CameraMove(RayCastData ray);

    //プレイヤーのワイヤーアクション時のステータスを設定
    void SetStatus(int type);
    
    //衝突判定
    void OnCollision(GameObject* pTarget) override;

    //エイム
    void Aim(RayCastData* ray);
    
    //エイムアシスト範囲内にあるかどうか
    bool IsAssistRange(const RayCastData& ray,const XMFLOAT3& targetVec,float length=9999.0f);
    
    //認識リスト内でエイムアシスト可能なやつをアシストする
    Enemy* AimAssist(RayCastData* rey);
 
    //プレイヤーのステータス取得
    char GetSatatus() { return status_; }
    
    //プレイヤーの移動ベクトル取得
    XMVECTOR GetPlayerMove() { return vPlayerMove_; }
    
    //パーティクル
    void OccurParticle();

    //敵をプレイヤーの認識リストに入れる(エイムアシストの対象を減らすため)
    void AddTargetList(Enemy* target);

    //認識リストにいるかどうかチェック
    void CheckTargetList();

    //開放
    void Release() override;




/////////////////////////////////セッター、ゲッター//////////////////////////////////
    XMVECTOR GetvBaseTarget() { return vBaseTarget_; }
    XMVECTOR GetvPlayerPos() { return vPlayerPos_; }
    XMMATRIX GetCameraMatrixX() { return matCamX_; }
    XMMATRIX GetCameraMatrixY() { return matCamY_; }
    XMMATRIX GetCameraMatrix() { return matCamY_*matCamX_; }
    Pointer* GetPointer() { return pPointer_; }
    float GetSpeed() { return speed_; }
    float GetLockOnAngleLimit() { return lockOnAngleLimit_; }
    std::list<Enemy*> GetEnemyList() { return enemyList_; }
    int GetLife() { return (int)playerLife_; }
    int GetTargetEnemyNum() { return enemyNumber_; }
    bool IsLockOn() { return lockOn_; }
    bool IsAim() { return aimFlag_; }
    bool IsFly() { return flyFlag_; }
    bool IsJump() { return jumpFlag_; }
    bool IsAir() { return airFlag_; }
    bool IsGround(){return groundFlag_;}
    void SetAreaLimit(XMFLOAT4 limit);
    void SetRotateSpeed(float rotateSpeed) { rotateSpeed_ = rotateSpeed; }
    void SetSpeed(float speed) { speed_ = speed; }
    void SetLockOnAngleLimit(float limit) { lockOnAngleLimit_ = limit; }
};