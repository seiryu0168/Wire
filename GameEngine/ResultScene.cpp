#include "ResultScene.h"
#include"ResultUI.h"

ResultScene::ResultScene(GameObject* parent)
	:GameObject(parent,"ResultScene")
{
}

ResultScene::~ResultScene()
{
}

void ResultScene::Initialize()
{
	Instantiate<ResultUI>(this);
}

void ResultScene::Update()
{
}

void ResultScene::Release()
{
}
