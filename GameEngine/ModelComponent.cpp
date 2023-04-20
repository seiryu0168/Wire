#include "ModelComponent.h"
#include"Engine/ResourceManager/Model.h"


ModelComponent::ModelComponent(std::string modelName,GameObject* obj)
	:Component(COMPONENT_TYPE::COMP_MODEL,obj)
{
	pFbxModel_ = nullptr;
	nowFrame_ = 0;
	animSpeed_ = 0;
	startFrame_ = 0;
	endFrame_ = 0;
	shaderType_ = SHADER_TYPE::SHADER_3D;
	Load(modelName);
}

ModelComponent::~ModelComponent()
{
}

void ModelComponent::Load(std::string fileName)
{
	//ModelManagerに同一モデルの有無を確認して、モデルがあった場合は
	//そのモデルのポインタを格納
	//なかった場合は読み込む
	pFbxModel_ = ModelManager::LoadModel(fileName);
}

void ModelComponent::SetTransform(const Transform* transform)
{
	//transformがnullptrだったら
	if (transform == nullptr)
	{
		transform_ = GetAttachObject()->GetTransform();
		return;
	}

		transform_ = *transform;
}

void ModelComponent::SetShader(SHADER_TYPE type)
{
	shaderType_ = type;
}

void ModelComponent::Update(bool active)
{
	if (active)
	{
		Draw();
	}
}

XMFLOAT3 ModelComponent::GetBonPosition(std::string boneName)
{
	XMFLOAT3 bonePos = pFbxModel_->GetBonePosition(boneName);
	XMVECTOR vBonePos = XMLoadFloat3(&bonePos)*GetAttachObject()->GetWorldMatrix();
	return StoreFloat3(vBonePos);
}

void ModelComponent::Draw()
{
	if (IsActive())
	{
		SetTransform();
		nowFrame_ += animSpeed_;
		if (nowFrame_ > endFrame_)
			nowFrame_ = startFrame_;
		pFbxModel_->Draw(transform_, shaderType_, (int)nowFrame_);
	}
}


void ModelComponent::SetAnimFrame(int start, int end, float speed)
{
	nowFrame_ = (float)start;
	startFrame_ = start;
	endFrame_ = end;
	animSpeed_ = speed;
}