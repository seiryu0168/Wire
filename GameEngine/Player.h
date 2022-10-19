#pragma once
#include "Engine/GameObject.h"
#include"Engine/Fbx.h"

class Player : public GameObject
{
    XMVECTOR rayDir[6];

    int hModel_;
    int stageNum_;
    bool flyFlag;
    XMVECTOR vCamPos;
    XMVECTOR vPlayerPos;
    //XMVECTOR vPlayerMove;
    XMVECTOR vBaseTarget;
    XMVECTOR vFlyMove;
    XMMATRIX matCam;
    float speed;
    float angleY;
    float angleX;

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

    void CharactorControll(XMVECTOR &moveVector);

    void CameraMove();

    void OnCollision(GameObject* pTarget) override; 
};

