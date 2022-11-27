#pragma once
#include "Engine/GameObject.h"
#include"Engine/Fbx.h"


class Player : public GameObject
{
    enum DIR_NAME
    {
        DIR_FRONT=0,
        DIR_BACK,
        DIR_LEFT,
        DIR_RIGHT,
        DIR_UP,
        DIR_DOWN
    };

    int playerLife_;

    XMVECTOR baseUpVec_;


    XMVECTOR rayDir_[6];

    const float gravity_;

    int hModel_;
    int stageNum_;

    bool flyFlag_;
    bool airFlag_;
    bool aimFlag_;
    bool jumpFlag_;
    bool groundFlag_;

    XMVECTOR vCamPos_;
    XMVECTOR vPlayerPos_;
    XMVECTOR vPlayerMove_;
    XMVECTOR vBaseTarget_;
    XMVECTOR vFlyMove_;
    XMVECTOR vBaseAim_;
    XMFLOAT3 flyMove_;
    XMMATRIX matCamX_;
    XMMATRIX matCamY_;

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

    char status_;

    //std::list<EnemyNormal*> enemyList_;


public:
    //コンストラクタ
    Player(GameObject* parent);

    //デストラクタ
    ~Player();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    void FixedUpdate() override;
    //描画
    void Draw() override;


    void CharactorControll(XMVECTOR &moveVector);

    void CameraMove(RayCastData ray);

    //プレイヤーのワイヤーアクション時のステータスを設定
    void SetStatus(int type);

    void OnCollision(GameObject* pTarget) override;

    bool IsAssistRange(XMVECTOR dirVec,XMFLOAT3 targetVec,float length=9999.0f);

    XMMATRIX AimAssist(XMFLOAT3 target, XMVECTOR frontVec=XMVectorSet(0,0,1,0),XMVECTOR upVector = XMVectorSet(0, 1, 0, 0));

    //開放
    void Release() override;


};

