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

void Wire::Init(XMVECTOR startPos, XMVECTOR endPos)
{

}

void Wire::Initialize()
{
	hModel_ = ModelManager::Load("Assets\\wire.fbx");
	assert(hModel_);
}

//�X�V
void Wire::Update()
{

}

void Wire::FixedUpdate()
{

}

//�`��
void Wire::Draw()
{
	ModelManager::SetTransform(hModel_, transform_);
	ModelManager::Draw(hModel_);
}

void Wire::Release()
{

}

void Wire::ExtendWire(const float& extendLength, XMMATRIX rotateMat)
{
	transform_.scale_.y = extendLength;

}