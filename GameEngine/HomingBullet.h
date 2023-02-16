#pragma once
#include"Engine/GameObject/GameObject.h"
#include"Engine/DirectX_11/BillBoard.h"
class Player;
class Particle;
class HomingBullet : public GameObject
{
    int hModel_;
    int hAudio_;
    int life_;
    int period_;
    XMVECTOR position_;
    XMVECTOR velocity_;
    Player* pPlayer_;
    BillBoard* pBill_;
    Particle* pParticle_;

public:
    //コンストラクタ
    HomingBullet(GameObject* parent);

    //デストラクタ
    ~HomingBullet();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    void FixedUpdate() override;

    void Homing();
    //描画
    void Draw() override;
    void BeforeDeath() override;

    void OnCollision(GameObject* target) override;

    void Release() override;
};

