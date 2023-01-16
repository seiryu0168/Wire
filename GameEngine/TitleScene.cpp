#include "TitleScene.h"
#include"Engine/Sprite.h"
#include"ObjectSetter.h"
#include"Engine/ImageManager.h"

TitleScene::TitleScene(GameObject* parent)
	: GameObject(parent, "TitleScene")
{
}

//初期化
void TitleScene::Initialize()
{
	Instantiate<ObjectSetter>(this);
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