#pragma once
#include"Player.h"
#include"Engine/DirectX_11/LineParticle.h"

enum class WIRE_STATE
{
    HOUSE = 0,
    STRETCH,
    EXTEND,
};

class Wire
{
private:

    LineParticle wireLine_;
    WIRE_STATE wireStatus_;
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
    void Update();
    //�`��
    void Draw(const Transform& transform);

    WIRE_STATE GetWireState() { return wireStatus_; }
    void ShotWire(XMVECTOR startPos, XMVECTOR endPos);
    void House();
    void Stretch();
    void Extend();
    void Release();
    int  CalcWidthSize();
    XMVECTOR GetWireVec() { return vWire_; }
    void ExtendWire(const float& extendLength,XMMATRIX rotateMat);
};

