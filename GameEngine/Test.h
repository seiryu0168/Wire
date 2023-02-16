#pragma once
#include "Engine/GameObject/GameObject.h"
#include"Engine/DirectX_11/Fbx.h"
#include"Engine/DirectX_11/BillBoard.h"
#include"Engine/DirectX_11/Particle.h"
class Test : public GameObject
{
    int hModel_;
    //Particle* pParticle_;
public:
    //コンストラクタ
    Test(GameObject* parent);

    //デストラクタ
    ~Test();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;
    void FixedUpdate() override;
    //描画
    void Draw() override;

    //開放
    void Release() override;

    void OnCollision(GameObject* pTarget) override;
};

