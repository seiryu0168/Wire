#pragma once
#include"Engine/GameObject/GameObject.h"
    enum ACTIONTYPE
    {
        ATC_DEFAULT = 0x00,
        ATC_MOVE    = 0x01,
        ATC_ATTACK  = 0x02,
        ATC_TOWALL  = 0x04,
        ATC_MAX
    };

class Pointer : public GameObject
{

private:
    int hModel_;
    bool drawFlag_;
    short objectType_;

public:

    Pointer(GameObject* parent);

    //デストラクタ
    ~Pointer();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    void FixedUpdate() override;
    //描画
    void Draw() override;

    //衝突
    void OnCollision(GameObject* pTarget) override;

    //開放
    void  Release() override;
    bool  IsDraw() { return drawFlag_; }
    void  SetDraw(bool flag) { drawFlag_ = flag; }
    short GetObjectType();
    void  SetPointerPos(XMFLOAT3 position);
};

