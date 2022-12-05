#include "Wire.h"
#include"Engine/Model.h"

Wire::Wire(GameObject* parent)
	:GameObject(parent,"Wire"),
	hModel_(-1)
{

}

Wire::~Wire()
{
}

void Wire::Initialize()
{
	hModel_ = Model::Load("Assets\\wire.fbx");
	assert(hModel_);
}

//çXêV
void Wire::Update()
{

}

void Wire::FixedUpdate()
{

}

//ï`âÊ
void Wire::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Wire::Release()
{

}

void Wire::ExtendWire(const float& extendLength, XMMATRIX rotateMat)
{
	transform_.scale_.y = extendLength;

	transform_.Rotation(rotateMat);

}