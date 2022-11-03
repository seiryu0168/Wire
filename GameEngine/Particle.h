#pragma once
#include"Engine/GameObject.h"

struct EmitterData
{
    
};
class Particle : public GameObject
{
private:
    int hModel_;
    float lifeTime_;
     XMFLOAT3 particleScale_;
     XMFLOAT3 particleRotate_;

public:
    Particle(GameObject* parent);

    //デストラクタ
    ~Particle();

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

