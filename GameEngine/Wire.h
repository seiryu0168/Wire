#pragma once
#include"Player.h"
#include"Engine/DirectX_11/LineParticle.h"
class Wire
{
private:
    LineParticle wireLine_;
    int extendFrame_;
    XMVECTOR vWire_;
    XMVECTOR vWidth_;
    int split_;
    int splitCount_;
    XMVECTOR startPos_;
    std::vector<float> shakeRate_;
public:
    Wire();

    //�f�X�g���N�^
    ~Wire();

    //������
    void Init(XMVECTOR startPos,XMVECTOR endPos);

    void SetWire(XMVECTOR startPos, XMVECTOR endPos);

    //�X�V
    int Update();
    //�`��
    void Draw(const Transform& transform);

    bool isExtend();
    void Release();
    int  CalcWidthSize();
    void ExtendWire(const float& extendLength,XMMATRIX rotateMat);
};

