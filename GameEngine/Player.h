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
    XMVECTOR rayDir_[6];
    int hModel_;
    int stageNum_;
    bool flyFlag_;
    bool airFlag_;

    XMVECTOR vCamPos_;
    XMVECTOR vPlayerPos_;
    XMVECTOR vBaseTarget_;
    XMVECTOR vFlyMove_;
    XMMATRIX matCamX_;
    XMMATRIX matCamY_;

    float velocity_;
    float speed_;
    float angleY_;
    float angleX_;

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

    //開放
    void Release() override;

    void Jump();

    void CharactorControll(XMVECTOR &moveVector);

    void CameraMove();

    void OnCollision(GameObject* pTarget) override; 
};

