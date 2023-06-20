#include "Stage1.h"
#include"Engine/ResourceManager/Model.h"
#include"ModelComponent.h"
namespace
{
	static const std::string FILE_NAME = "Stagename.json";
}
//コンストラクタ
Stage::Stage(GameObject* parent)
	:GameObject(parent,"Stage1"),
	hModel_(-1)
{
	stageTable_.Initialize(FILE_NAME);
}

//デストラクタ
Stage::~Stage()
{

}

//初期化
void Stage::Initialize()
{
	hModel_=stageTable_.LoadStage(stageNum_);
	//ModelComponent* mComp = new ModelComponent("Assets\\NewStage_Maya.fbx", this);
	//AddComponent(mComp);
	//ステージテーブルからステージのモデル名をとってくる
	//hModel_ = ModelManager::Load("Assets\\NewStage_Maya.fbx");
	assert(hModel_ >= 0);
	transform_.position_ = XMFLOAT3(0, 0, 0);
}

//更新
void Stage::Update() 
{

}

void Stage::FixedUpdate()
{

}

//描画
void Stage::Draw()
{
	ModelManager::SetTransform(hModel_, transform_);
		ModelManager::Draw(hModel_);
}

//開放
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
