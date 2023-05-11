#include "Stage1.h"
#include"Engine/ResourceManager/Model.h"
#include"ModelComponent.h"
namespace
{
	static const std::string FILE_NAME = "Stagename.json";
}
//コンストラクタ
Stage1::Stage1(GameObject* parent)
	:GameObject(parent,"Stage1"),
	hModel_(-1)
{
	stageTable_.Initialize(FILE_NAME);
}

//デストラクタ
Stage1::~Stage1()
{

}

//初期化
void Stage1::Initialize()
{
	hModel_=stageTable_.LoadStage(1);
	//ModelComponent* mComp = new ModelComponent("Assets\\NewStage_Maya.fbx", this);
	//AddComponent(mComp);
	//ステージテーブルからステージのモデル名をとってくる
	//hModel_ = ModelManager::Load("Assets\\NewStage_Maya.fbx");
	assert(hModel_ >= 0);
	transform_.position_ = XMFLOAT3(0, 0, 0);
}

//更新
void Stage1::Update() 
{

}

void Stage1::FixedUpdate()
{

}

//描画
void Stage1::Draw()
{
	ModelManager::SetTransform(hModel_, transform_);
	ModelManager::Draw(hModel_);
}

//開放
void Stage1::Release()
{

}

void Stage1::LoadStageData()
{
	//stageTable_.LoadStage(0);
}
