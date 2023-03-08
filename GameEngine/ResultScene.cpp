#include "ResultScene.h"
#include"ObjectSetter.h"

ResultScene::ResultScene(GameObject* parent)
	:GameObject(parent,"ResultScene")
{
}

ResultScene::~ResultScene()
{
}

void ResultScene::Initialize()
{
	Instantiate<ObjectSetter>(this);
}

void ResultScene::Update()
{
}

void ResultScene::Release()
{
}
