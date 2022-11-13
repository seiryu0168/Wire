#pragma once
#include"Engine/GameObject.h" 
#include<vector>
class SetObject : public GameObject
{
private:
    struct objectData
    {
        std::string objectClassName;
        Transform objectTransform;
    
    };
    std::vector<objectData> objectDataList_;
public:
    SetObject(GameObject* parent);

    //デストラクタ
    ~SetObject();

    //初期化
    void Initialize() override;

    ////更新
    //void Update() override;

    //void FixedUpdate() override;
    ////描画
    //void Draw() override;

    void Load();

    void ObjectSet();

    //開放
    void Release() override;
};

