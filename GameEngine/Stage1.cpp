#include "Stage1.h"
#include"Engine/ResourceManager/Model.h"
//コンストラクタ
Stage1::Stage1(GameObject* parent)
	:GameObject(parent,"Stage1"),
	hModel_(-1)
{

}

//デストラクタ
Stage1::~Stage1()
{

}

//初期化
void Stage1::Initialize()
{
	hModel_ = ModelManager::Load("Assets\\NewStage_Maya.fbx");
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