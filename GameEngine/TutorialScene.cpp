#include "TutorialScene.h"
#include"ObjectSetter.h"
TutorialScene::TutorialScene(GameObject* parent)
	:GameObject(parent,"TutorialScene")
{
}

TutorialScene::~TutorialScene()
{
}

void TutorialScene::Initialize()
{
	Instantiate<ObjectSetter>(this);
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
