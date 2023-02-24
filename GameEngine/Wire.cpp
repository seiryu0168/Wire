#include "Wire.h"
#include"Easing.h"
namespace
{
	static const int FRAME = 15;

}
Wire::Wire()
	:splitCount_(5),
	split_(0),
	extendFrame_(-1),
	wireStatus_(WIRE_STATE::HOUSE),
	startPos_(XMVectorZero()),
	vWidth_(XMVectorZero()),
	vWire_(XMVectorZero())
{
	wireLine_.SetLineParameter(1.0f, splitCount_);
	wireLine_.Load("Assets\\Effect01.png");
}

Wire::~Wire()
{
}

void Wire::Init(XMVECTOR startPos, XMVECTOR endPos)
{
	
}

void Wire::SetWire(XMVECTOR startPos, XMVECTOR endPos)
{
	extendFrame_ = FRAME;
	startPos_ = startPos;
	vWire_ = endPos - startPos;
	split_ = XMVectorGetX(XMVector3Length(vWire_))/(float)splitCount_;
	vWire_ = XMVector3Normalize(vWire_);
	vWidth_ = XMVector3Normalize(XMVector3Cross(vWire_, XMVectorSet(0, 1, 0, 0)));
}

//XV
void Wire::Update()
{
	XMFLOAT3 pos;
	switch (wireStatus_)
	{
	case WIRE_STATE::HOUSE:
		House();
		break;

	case WIRE_STATE::STRETCH:
		Stretch();
		break;

	case WIRE_STATE::EXTEND:
		Extend();
		break;

	default:
		break;
	}
}

//•`‰æ
void Wire::Draw(const Transform& transform)
{
	if(extendFrame_ > 0)
	wireLine_.Draw((Transform*)&transform);
}

void Wire::ShotWire(XMVECTOR startPos, XMVECTOR endPos)
{
	wireStatus_ = WIRE_STATE::STRETCH;
	SetWire(startPos, endPos);
}

void Wire::House()
{
}

void Wire::Stretch()
{
	XMFLOAT3 pos;
	if (extendFrame_ > 0)
	{
		for (int i = 0; i < splitCount_; i++)
		{
			XMStoreFloat3(&pos, (startPos_ + (vWire_ * split_ * i*Easing::EaseINOutQuad((float)(FRAME-extendFrame_)/(float)FRAME))) + vWidth_ * CalcWidthSize());
			vWidth_ *= -1;
			wireLine_.AddPosition(pos);
		}
		vWidth_ *= -1;
	}
	extendFrame_--;
	extendFrame_ = max(extendFrame_, 0);
	if (extendFrame_ == 0)
	{
		wireStatus_ = WIRE_STATE::EXTEND;
	}
}

void Wire::Extend()
{
	wireStatus_ = WIRE_STATE::HOUSE;
}

void Wire::Release()
{

}

int Wire::CalcWidthSize()
{
	float widthSize = ((float)extendFrame_ / (float)FRAME) * Easing::EaseOutQuad((float)splitCount_ / 5.0f) * 3;
	return widthSize;
}

void Wire::ExtendWire(const float& extendLength, XMMATRIX rotateMat)
{
	
}