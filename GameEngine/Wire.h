#pragma once
#include"Player.h"
class Wire
{
private:
    int hModel_;
    int extendFrame
public:
    Wire(GameObject* parent);

    //デストラクタ
    ~Wire();

    //初期化
    void Init(XMVECTOR startPos,XMVECTOR endPos);

    //更新
    void Update();
    //描画
    void Draw();

    void Release();

    void ExtendWire(const float& extendLength,XMMATRIX rotateMat);
};

