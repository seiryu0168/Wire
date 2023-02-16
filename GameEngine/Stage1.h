#pragma once
#include"Engine/GameObject/GameObject.h"
class Stage1 : public GameObject
{
private:
    int hModel_;

public:
    //コンストラクタ
    Stage1(GameObject* parent);

    //デストラクタ
    ~Stage1();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    void FixedUpdate() override;
    //描画
    void Draw() override;

    //開放
    void Release() override;

    int GetModelHandle() { return hModel_; }
};

