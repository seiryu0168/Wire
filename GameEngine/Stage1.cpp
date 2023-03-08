#include "Stage1.h"
#include"Engine/ResourceManager/Model.h"
//�R���X�g���N�^
Stage1::Stage1(GameObject* parent)
	:GameObject(parent,"Stage1"),
	hModel_(-1)
{

}

//�f�X�g���N�^
Stage1::~Stage1()
{

}

//������
void Stage1::Initialize()
{
	hModel_ = ModelManager::Load("Assets\\NewStage_Maya.fbx");
	assert(hModel_ >= 0);
	transform_.position_ = XMFLOAT3(0, 0, 0);
}

//�X�V
void Stage1::Update() 
{

}

void Stage1::FixedUpdate()
{

}

//�`��
void Stage1::Draw()
{
	ModelManager::SetTransform(hModel_, transform_);
	ModelManager::Draw(hModel_);
}

//�J��
void Stage1::Release()
{

}