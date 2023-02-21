#include "Wire.h"
#include"Easing.h"
namespace
{
	static const int FRAME = 10;

}
Wire::Wire()
	:splitCount_(5),
	extendFrame_(-1)
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
int Wire::Update()
{
	XMFLOAT3 pos;
	if (extendFrame_ > 0)
	{
		for (int i = 0; i < splitCount_; i++)
		{
			XMStoreFloat3(&pos, (startPos_ + (vWire_ * split_ * i)) + vWidth_ * CalcWidthSize());
			vWidth_ *= -1;
			wireLine_.AddPosition(pos);
		}
		vWidth_ *= -1;
	}
	extendFrame_--;
	extendFrame_ = max(extendFrame_, -1);
	return extendFrame_;
}

//•`‰æ
void Wire::Draw(const Transform& transform)
{
	if(extendFrame_ > 0)
	wireLine_.Draw((Transform*)&transform);
}

bool Wire::isExtend()
{
	if (extendFrame_ <= 0)
		return true;
	return false;
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