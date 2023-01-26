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
	if (InterSceneData::GetboolData("Result"))
	hPict_ = ImageManager::Load("Assets\\Clear.png");

	else if (InterSceneData::GetboolData("Result")==false)
		hPict_ = ImageManager::Load("Assets\\GameOver.png");
}

void ResultUI::Update()
{
}

void ResultUI::Release()
{
}
