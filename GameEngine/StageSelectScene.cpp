#include "StageSelectScene.h"
#include"ObjectSetter.h"
StageSelectScene::StageSelectScene(GameObject* parent)
	:SceneBase(parent,"StageSelectScene")
{
	Instantiate<ObjectSetter>(this);
}

StageSelectScene::~StageSelectScene()
{
}

void StageSelectScene::Initialize()
{
}

void StageSelectScene::Update()
{
}

void StageSelectScene::Draw()
{
}

void StageSelectScene::Release()
{
}
