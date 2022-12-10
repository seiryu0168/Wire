#pragma once
#include "Engine/GameObject.h"
class Bullet : public GameObject
{
private:
    int hModel_;
    float speed_;

    int life_;
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

