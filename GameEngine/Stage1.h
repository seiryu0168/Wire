#pragma once
#include"Engine/GameObject/GameObject.h"
#include"StageManager.h"
class Stage1 : public GameObject
{
private:
    int hModel_;
    int stageNum_;
    StageManager stageTable_;
public:
    //�R���X�g���N�^
    Stage1(GameObject* parent);

    //�f�X�g���N�^
    ~Stage1();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    void FixedUpdate() override;
    //�`��
    void Draw() override;

    //�J��
    void Release() override;

    void LoadStageData();

    int GetModelHandle() { return hModel_; }
};

