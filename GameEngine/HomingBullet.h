#pragma once
#include"Engine/GameObject.h"
#include"Engine/BillBoard.h"
class Player;
class Particle;
class HomingBullet : public GameObject
{
    int hModel_;
    int life_;
    int period_;
    float speed_;
    XMVECTOR position_;
    XMVECTOR velocity_;
    Player* pPlayer_;
    BillBoard* pBill_;
    Particle* pParticle_;
    XMVECTOR dir_;

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
    void SetDir(XMVECTOR dir) { dir_ = dir; }
    //描画
    void Draw() override;

    void OnCollision(GameObject* target) override;

    void Release() override;
};

