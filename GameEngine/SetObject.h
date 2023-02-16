#pragma once
#include"Engine/GameObject/GameObject.h" 
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

    //�f�X�g���N�^
    ~SetObject();

    //������
    void Initialize() override;

    ////�X�V
    //void Update() override;

    //void FixedUpdate() override;
    ////�`��
    //void Draw() override;

    void Load();

    void ObjectSet();

    //�J��
    void Release() override;
};

