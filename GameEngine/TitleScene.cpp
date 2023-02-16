#include "TitleScene.h"
#include"Engine/DirectX_11/Sprite.h"
#include"ObjectSetter.h"
#include"Engine/ResourceManager/ImageManager.h"

TitleScene::TitleScene(GameObject* parent)
	: GameObject(parent, "TitleScene")
{
}

//‰Šú‰»
void TitleScene::Initialize()
{
	Instantiate<ObjectSetter>(this);
}

//XV
void TitleScene::Update()
{

	
}

void TitleScene::FixedUpdate()
{

}

//•`‰æ
void TitleScene::Draw()
{
}

//ŠJ•ú
void TitleScene::Release()
{
}