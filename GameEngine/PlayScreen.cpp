#include "PlayScreen.h"
#include"Engine/ResourceManager/ImageManager.h"
#include"ObjectSetter.h"
#include"TutorialOrder.h"
#include"MissionOrder.h"
PlayScreen::PlayScreen(GameObject* parent)
	:GameObject(parent,"PlayScreen"),
	pUI(nullptr)
{
}

PlayScreen::~PlayScreen()
{
}

void PlayScreen::Initialize()
{
	hPict_[0] = ImageManager::Load("Assets\\ScreenFrameUp2.png");
	assert(hPict_[0] >= 0);
	hPict_[1] = ImageManager::Load("Assets\\ScreenFrameDown2.png");
	assert(hPict_[1] >= 0);
	hPict_[2] = ImageManager::Load("Assets\\ScreenFrameLeft2.png");
	assert(hPict_[2] >= 0);
	hPict_[3] = ImageManager::Load("Assets\\ScreenFrameRight2.png");
	assert(hPict_[3] >= 0);

	//データを外部に出せるようになったら変える
	ImageManager::SetImagePos(hPict_[0], { 0,580,0 });
	ImageManager::SetImagePos(hPict_[1], { 0,-580,0 });
	ImageManager::SetImagePos(hPict_[2], { -1410,10,0 });
	ImageManager::SetImagePos(hPict_[3], { 1410,10,0 });
	ObjectSetter* p = (ObjectSetter*)FindObject("ObjectSetter");
	
	if (p->GetSceneName() == "PlayScene")
	{
		pUI = new MissionOrder(p);
	}
	else if (p->GetSceneName() == "TutorialScene")
	{
		pUI = new TutorialOrder();
	}
}

void PlayScreen::Update()
{
	pUI->Update();
}

void PlayScreen::Draw()
{
}
void PlayScreen::ThirdDraw()
{
	pUI->Draw();
}

void PlayScreen::Release()
{
	pUI->Release();
}

void PlayScreen::SetDir(XMFLOAT2 dir)
{
	ImageManager::SetAlpha(hPict_[0], 0);
	ImageManager::SetAlpha(hPict_[1], 0);
	ImageManager::SetAlpha(hPict_[2], 0);
	ImageManager::SetAlpha(hPict_[3], 0);

	if (dir.x >= 0.4)
	{
		ImageManager::SetAlpha(hPict_[3], 1);
	}
	if (dir.x <= -0.4)
	{
		ImageManager::SetAlpha(hPict_[2], 1);
	}
	if (dir.y >= 0.4)
	{
		ImageManager::SetAlpha(hPict_[0], 1);
	}
	if (dir.y <= -0.4)
	{
		ImageManager::SetAlpha(hPict_[1], 1);
	}
}
