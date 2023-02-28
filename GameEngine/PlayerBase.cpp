#include "PlayerBase.h"
#include"Engine/ResourceManager/Model.h"
PlayerBase::PlayerBase(GameObject* parent)
	:GameObject(parent,"PlayerBase"),
	hModel_(-1)
{
}

PlayerBase::~PlayerBase()
{
}

void PlayerBase::Initialize()
{
	hModel_ = ModelManager::Load("Assets\\Player.fbx");
	assert(hModel_ >= 0);
}

void PlayerBase::Update()
{
}

void PlayerBase::Draw()
{
	ModelManager::SetTransform(hModel_, transform_);
	ModelManager::Draw(hModel_);

}

void PlayerBase::Release()
{
}
