#pragma once
#include"Engine/GameObject/GameObject.h"
#include"StageManager.h"
class Stage : public GameObject
{
private:
    int hModel_;
    int stageNum_;
    StageManager stageTable_;
public:
    //�R���X�g���N�^
    Stage(GameObject* parent);

    //�f�X�g���N�^
    ~Stage();

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
    void SetStageNum(int stageNum);
    XMFLOAT4 GetStageAreaLimit();
    int GetModelHandle() { return hModel_; }
};

