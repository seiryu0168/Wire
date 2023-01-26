#include "ResultUI.h"
#include"Engine/ImageManager.h"
#include"InterSceneData.h"
ResultUI::ResultUI(GameObject* parent)
	:GameObject(parent,"ResultUI"),
	hPict_(-1),
	hPictTitle_(-1),
	hPictRetry_(-1)
{
}

ResultUI::~ResultUI()
{
}

void ResultUI::Initialize()
{
	if (InterSceneData::GetintData("PlayerData") == 0)
	hPict_ = ImageManager::Load("Assets\\GameOver.png");

	else if (InterSceneData::GetintData("PlayerData") > 0)
		hPict_ = ImageManager::Load("Assets\\Clear.png");
}

void ResultUI::Update()
{
}

void ResultUI::Release()
{
}
