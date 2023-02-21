#pragma once
#include"Player.h"
class Wire
{
private:
    int hModel_;
    int extendFrame
public:
    Wire(GameObject* parent);

    //�f�X�g���N�^
    ~Wire();

    //������
    void Init(XMVECTOR startPos,XMVECTOR endPos);

    //�X�V
    void Update();
    //�`��
    void Draw();

    void Release();

    void ExtendWire(const float& extendLength,XMMATRIX rotateMat);
};

