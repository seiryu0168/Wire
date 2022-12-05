#pragma once
#include"Engine/GameObject.h"
class Wire : public GameObject
{
private:
    int hModel_;

public:
    Wire(GameObject* parent);

    //デストラクタ
    ~Wire();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    void FixedUpdate() override;
    //描画
    void Draw() override;

    void Release() override;

    void ExtendWire(const float& extendLength,XMMATRIX rotateMat);
};

