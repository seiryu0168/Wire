#pragma once
#include "Engine/GameObject.h"
#include"Engine/BillBoard.h"
class Bullet : public GameObject
{
private:
    int hModel_;
    int life_;
    float speed_;

    BillBoard* pBill_;
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

    void FixedUpdate() override;

    void SetDir(XMVECTOR dir) { dir_ = dir; }
    //描画
    void Draw() override;
    
    void OnCollision(GameObject* target) override;

    void Release() override;
};

