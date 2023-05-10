#include "Stage1.h"
#include"Engine/ResourceManager/Model.h"
#include"ModelComponent.h"
#include"Engine/ResourceManager/json.hpp"
using json = nlohmann::json;
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
	//ModelComponent* mComp = new ModelComponent("Assets\\NewStage_Maya.fbx", this);
	//AddComponent(mComp);
	//�X�e�[�W�e�[�u������X�e�[�W�̃��f�������Ƃ��Ă���
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

void Stage1::LoadStageData()
{
	json j;
	j["stage1"] = "stage1.fbx";
	std::string jstr = R"({"position":[{1,23,1.0f},{0,0,0},{1.0,1.0,1.0}]})";

	j["transform"] = json::parse(jstr);




}
