#include "PlayScreen.h"
#include"Engine/ResourceManager/ImageManager.h"
#include"Player.h"
#include"ObjectSetter.h"
#include"TutorialOrder.h"
#include"MissionOrder.h"
namespace
{
	static const int LOW_HP = 3;

}
PlayScreen::PlayScreen(GameObject* parent)
	:GameObject(parent,"PlayScreen"),
	pUI(nullptr),
	hLowHPPict_(-1)
{
	for (int i : hPict_)
	{
		i = -1;

	}
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
	hLowHPPict_ = ImageManager::Load("Assets\\LowHPScreen.png");
	assert(hLowHPPict_ >= 0);


	//データを外部に出せるようになったら変える
	ImageManager::SetImagePos(hPict_[0], { 0,580,0 });
	ImageManager::SetImagePos(hPict_[1], { 0,-580,0 });
	ImageManager::SetImagePos(hPict_[2], { -1410,10,0 });
	ImageManager::SetImagePos(hPict_[3], { 1410,10,0 });
	for (int i : hPict_)
	{
		ImageManager::SetUIList(i);
	}
	ObjectSetter* p = (ObjectSetter*)FindObject("ObjectSetter");
	
	/*if (p->GetSceneName() == "PlayScene")
	{
		pUI = new MissionOrder(p);
	}
	else if (p->GetSceneName() == "TutorialScene")
	{
		pUI = new TutorialOrder();
	}*/
}

void PlayScreen::Update()
{
	//pUI->Update();
}

void PlayScreen::Draw()
{
	if (((Player*)GetParent())->GetLife() <= LOW_HP)
	{
		ImageManager::Draw(hLowHPPict_);
	}
}
void PlayScreen::ThirdDraw()
{
	//pUI->Draw();
}

void PlayScreen::Release()
{
	//pUI->Release();
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
