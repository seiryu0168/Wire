#pragma once
#include "Engine//GameObject.h"
#include"Engine/Fbx.h"

class Test : public GameObject
{
    int hModel_;
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

