#pragma once
#include"Engine/GameObject/GameObject.h"
class Wire : public GameObject
{
private:
    int hModel_;

public:
    Wire(GameObject* parent);

    //�f�X�g���N�^
    ~Wire();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    void FixedUpdate() override;
    //�`��
    void Draw() override;

    void Release() override;

    void ExtendWire(const float& extendLength,XMMATRIX rotateMat);
};

