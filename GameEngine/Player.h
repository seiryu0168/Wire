#pragma once
#include "Engine/GameObject.h"
#include"Engine/Fbx.h"

class Player : public GameObject
{
    int hModel_;
    XMVECTOR vCamPos;
    XMVECTOR vPlayerPos;
    XMMATRIX matCam;
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

    void CameraMove();

    void OnCollision(GameObject* pTarget) override; 
};

