#pragma once
#include "Engine/GameObject/GameObject.h"
#include"Engine/DirectX_11/BillBoard.h"

class Particle;
class Bullet : public GameObject
{
private:
    int hModel_;
    int hAudio_;
    int life_;
    float speed_;

    BillBoard* pBill_;
    Particle* pParticle_;
    XMVECTOR dir_;

public:
    //コンストラクタ
    Bullet(GameObject* parent);

    //デストラクタ
    ~Bullet();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    void SetDir(XMVECTOR dir) { dir_ = dir; }
    //描画
    void Draw() override;
    
    //衝突時
    void OnCollision(GameObject* target) override;

    void Release() override;
};

