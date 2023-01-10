#include "TitleScene.h"
#include"ObjectSetter.h"
#include"Engine/Sprite.h"
#include"Engine/Sprite.h"

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
	Sprite a;
	a.Initialize();
	a.Draw(transform_);

}

//ŠJ•ú
void TitleScene::Release()
{
}