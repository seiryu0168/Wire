#include "ResultScene.h"
#include"ObjectSetter.h"

ResultScene::ResultScene(GameObject* parent)
	:SceneBase(parent,"ResultScene")
{
	Instantiate<ObjectSetter>(this);
}

ResultScene::~ResultScene()
{
}

void ResultScene::Initialize()
{
}

void ResultScene::Update()
{
}

void ResultScene::Release()
{
}
