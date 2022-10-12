#include "TitleScene.h"
#include"Player.h"
#include"Engine/Model.h"
#include"Test.h"
#include"Engine/Fbx.h"
TitleScene::TitleScene(GameObject* parent)
	: GameObject(parent, "TitleScene")
{
}

//初期化
void TitleScene::Initialize()
{
	Instantiate<Test>(this);
	Instantiate<Player>(this);
}

//更新
void TitleScene::Update()
{
	
}

void TitleScene::FixedUpdate()
{

}

//描画
void TitleScene::Draw()
{
}

//開放
void TitleScene::Release()
{
}