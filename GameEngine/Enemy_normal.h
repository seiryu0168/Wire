#pragma once
#include"Enemy.h"
//#include"Engine/GameObject.h"
class Enemy_normal : public Enemy
{
public:
    int hModel_;
private:
    //コンストラクタ
    Enemy_normal(GameObject* parent);

    //デストラクタ
    ~Enemy_normal();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    void FixedUpdate() override;
    //描画
    void Draw() override;

    //開放
    void Release() override;
};