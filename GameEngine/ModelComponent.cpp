#include "ModelComponent.h"
#include"Engine/ResourceManager/Model.h"


ModelComponent::ModelComponent(std::string modelName)
	:Component(COMPONENT_TYPE::COMP_MODEL)
{
	pFbxModel_ = nullptr;
	nowFrame_ = 0;
	animSpeed_ = 0;
	startFrame_ = 0;
	endFrame_ = 0;
	//hModel_ = ModelManager::Load(modelName);
}

ModelComponent::~ModelComponent()
{
}

void ModelComponent::SetAnimFrame(int start, int end, float speed)
{
	nowFrame_ = (float)start;
	startFrame_ = start;
	endFrame_ = end;
	animSpeed_ = speed;
}