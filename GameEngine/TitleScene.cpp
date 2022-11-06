#include "TitleScene.h"
#include"Engine/Model.h"
#include"Engine/Fbx.h"
#include"Player.h"
#include"Test.h"
#include"Stage1.h"

TitleScene::TitleScene(GameObject* parent)
	: GameObject(parent, "TitleScene")
{
}

//‰Šú‰»
void TitleScene::Initialize()
{
	Instantiate<Stage1>(this);
	Instantiate<Player>(this);
	Instantiate<Test>(this);
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