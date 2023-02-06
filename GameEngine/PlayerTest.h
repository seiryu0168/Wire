#pragma once
#include "Engine/GameObject.h"
#include"Engine/Fbx.h"
#include"Engine/LineParticle.h"
#include"PlayerState.h"
    class Particle;
    class Enemy;
    class Wire;
    class Pointer;
    class PlayerStatus;
    class ObjectSetter;
class PlayerTest : public GameObject
{
    PlayerState<PlayerTest>* pState_;
    class StateJump : public PlayerState<PlayerTest>
    {
    public:
        static StateJump* GetInstance()
        {
            static StateJump* instance = nullptr;
            if (instance = nullptr)
            {
                instance = new StateJump();
            }
            return instance;
        }
    };

        enum DIR_NAME
        {
            DIR_FRONT = 0,
            DIR_BACK,
            DIR_LEFT,
            DIR_RIGHT,
            DIR_UP,
            DIR_DOWN
        };
        enum STATUS
        {
            STATE_GROUND = 0,
            STATE_AIR,
            STATE_FLY,
        };

        XMVECTOR baseUpVec_;
        XMVECTOR rayDir_[6];

        char status_;

        const float gravity_;

        bool flyFlag_;
        bool airFlag_;
        bool aimFlag_;
        bool jumpFlag_;
        bool groundFlag_;
        bool godFlag_;

        int hModel_;
        int hModel_Handle_;
        int hAudio_;
        int PlayerTestLife_;
        int stageNum_;
        int godTime_;
        std::vector<int> life_;

        float moveTime_;
        float aimTime_;
        float flyTime_;
        float velocity_;
        float rotateSpeed_;
        float maxSpeed_;
        float wireLength_;
        float angleY_;
        float angleX_;
        float lockOnAngleLimit_;


        XMVECTOR vCamPos_;
        XMVECTOR vPlayerTestPos_;
        XMVECTOR vPlayerTestMove_;
        XMVECTOR vBaseTarget_;
        XMVECTOR vFlyMove_;
        XMVECTOR vBaseAim_;
        XMFLOAT3 flyMove_;
        XMMATRIX matCamX_;
        XMMATRIX matCamY_;



        PlayerTestStatus* PlayerTestState_;
        Particle* pParticle_;
        LineParticle* pLine_;
        LineParticle* pWire_;
        Pointer* pPointer_;
        ObjectSetter* pSetter_;
        std::list<Enemy*> enemyList_;


    public:
        //コンストラクタ
        PlayerTest(GameObject* parent);

        //デストラクタ
        ~PlayerTest();

        //初期化
        void Initialize() override;

        //更新
        void Update() override;

        void FixedUpdate() override;
        //描画
        void Draw() override;

        void SecondDraw() override;

        //キャラが壁をすり抜けないようにする
        void CharactorControll(XMVECTOR& moveVector);

        //カメラの挙動を制御
        void CameraMove(RayCastData ray);

        //プレイヤーのワイヤーアクション時のステータスを設定
        void SetStatus(int type);

        //衝突判定
        void OnCollision(GameObject* pTarget) override;

        //エイムアシスト範囲内にあるかどうか
        bool IsAssistRange(XMVECTOR dirVec, XMFLOAT3 targetVec, float length = 9999.0f);

        //認識リスト内でエイムアシスト可能なやつをアシストする
        Enemy* AimAssist(RayCastData* rey);

        //プレイヤーのステータス取得
        char GetSatatus() { return status_; }

        //プレイヤーの移動ベクトル取得
        XMVECTOR GetPlayerTestMove() { return vPlayerTestMove_; }

        //パーティクル
        void OccurParticle();

        //敵をプレイヤーの認識リストに入れる(エイムアシストの対象を減らすため)
        void AddTargetList(Enemy* target);

        //認識リストにいるかどうかチェック
        void CheckTargetList();

        //敵をプレイヤーの認識リストから外す
        void DeleteTargetList(Enemy* target);
        //開放
        void Release() override;




        /////////////////////////////////セッター、ゲッター//////////////////////////////////
        XMVECTOR GetvBaseTarget() { return vBaseTarget_; }
        XMVECTOR GetvPlayerTestPos() { return vPlayerTestPos_; }
        XMMATRIX GetCameraMatrixX() { return matCamX_; }
        XMMATRIX GetCameraMatrixY() { return matCamY_; }
        XMMATRIX GetCameraMatrix() { return matCamY_ * matCamX_; }
        Pointer* GetPointer() { return pPointer_; }
        std::list<Enemy*> GetEnemyList() { return enemyList_; }

        void SetRotateSpeed(float rotateSpeed) { rotateSpeed_ = rotateSpeed; }
};

