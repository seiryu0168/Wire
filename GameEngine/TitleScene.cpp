#include "TitleScene.h"
#include"Engine/DirectX_11/Sprite.h"
#include"ObjectSetter.h"
#include"Engine/ResourceManager/ImageManager.h"

TitleScene::TitleScene(GameObject* parent)
	: SceneBase(parent, "TitleScene")
{
	Instantiate<ObjectSetter>(this);
}

//初期化
void TitleScene::Initialize()
{
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