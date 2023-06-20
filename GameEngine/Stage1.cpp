#include "Stage1.h"
#include"Engine/ResourceManager/Model.h"
#include"ModelComponent.h"
namespace
{
	static const std::string FILE_NAME = "Stagename.json";
}
//�R���X�g���N�^
Stage::Stage(GameObject* parent)
	:GameObject(parent,"Stage1"),
	hModel_(-1)
{
	stageTable_.Initialize(FILE_NAME);
}

//�f�X�g���N�^
Stage::~Stage()
{

}

//������
void Stage::Initialize()
{
	hModel_=stageTable_.LoadStage(stageNum_);
	//ModelComponent* mComp = new ModelComponent("Assets\\NewStage_Maya.fbx", this);
	//AddComponent(mComp);
	//�X�e�[�W�e�[�u������X�e�[�W�̃��f�������Ƃ��Ă���
	//hModel_ = ModelManager::Load("Assets\\NewStage_Maya.fbx");
	assert(hModel_ >= 0);
	transform_.position_ = XMFLOAT3(0, 0, 0);
}

//�X�V
void Stage::Update() 
{

}

void Stage::FixedUpdate()
{

}

//�`��
void Stage::Draw()
{
	ModelManager::SetTransform(hModel_, transform_);
		ModelManager::Draw(hModel_);
}

//�J��
void Stage::Release()
{

}

void Stage::LoadStageData()
{
	//stageTable_.LoadStage(0);
}

void Stage::SetStageNum(int stageNum)
{
	if (stageNum < stageTable_.MaxStage())
		stageNum_ = stageNum;
	else
		stageNum_ = (stageTable_.MaxStage() - 1);
}

XMFLOAT4 Stage::GetStageAreaLimit()
{
	return stageTable_.GetAreaLimit(stageNum_);
}
