#include "TutorialScene.h"
#include"ObjectSetter.h"
TutorialScene::TutorialScene(GameObject* parent)
	:SceneBase(parent,"TutorialScene")
{
	Instantiate<ObjectSetter>(this);
}

TutorialScene::~TutorialScene()
{
}

void TutorialScene::Initialize()
{
}

void TutorialScene::Update()
{
}

void TutorialScene::Draw()
{
}

void TutorialScene::Release()
{
}
