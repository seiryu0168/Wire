#pragma once
#include"Engine/GameObject/GameObject.h"
#include<vector>
class Stage1 : public GameObject
{
private:
    int hModel_;
    int stageNum_;
    std::vector<std::string> stageName_;
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

    void LoadStageData();

    int GetModelHandle() { return hModel_; }
};

