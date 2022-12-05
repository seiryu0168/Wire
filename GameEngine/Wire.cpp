#include "Wire.h"
#include"Engine/Model.h"

Wire::Wire(GameObject* parent)
	:hModel_(-1)
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

}

void Wire::ExtendWire(XMVECTOR scale)
{

}