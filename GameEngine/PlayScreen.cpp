#include "PlayScreen.h"
#include"Engine/ResourceManager/ImageManager.h"
#include"Player.h"
#include"ObjectSetter.h"
#include"TutorialOrder.h"
#include"MissionOrder.h"
namespace
{
	static const int LOW_HP = 3;
	static const float TILT = 0.4f;
	static const std::string SCREEN_FRAME_UP = "Assets\\Image\\ScreenFrameUp2.png";
	static const std::string SCREEN_FRAME_DOWN = "Assets\\Image\\ScreenFrameDown2.png";
	static const std::string SCREEN_FRAME_LEFT = "Assets\\Image\\ScreenFrameLeft2.png";
	static const std::string SCREEN_FRAME_RIGHT = "Assets\\Image\\ScreenFrameRight2.png";
	static const std::string LOW_SCREEN_IMAGE = "Assets\\Image\\LowHPScreen.png";

}
PlayScreen::PlayScreen(GameObject* parent)
	:GameObject(parent,"PlayScreen"),
	hLowHPPict_(-1),
	pUI(nullptr)
{
	hPict_[0] = -1;
	hPict_[1] = -1;
	hPict_[2] = -1;
	hPict_[3] = -1;
}

PlayScreen::~PlayScreen()
{
}

void PlayScreen::Initialize()
{
	hPict_[0] = ImageManager::Load(SCREEN_FRAME_UP);
	assert(hPict_[0] >= 0);
	hPict_[1] = ImageManager::Load(SCREEN_FRAME_DOWN);
	assert(hPict_[1] >= 0);
	hPict_[2] = ImageManager::Load(SCREEN_FRAME_LEFT);
	assert(hPict_[2] >= 0);
	hPict_[3] = ImageManager::Load(SCREEN_FRAME_RIGHT);
	assert(hPict_[3] >= 0);
	hLowHPPict_ = ImageManager::Load(LOW_SCREEN_IMAGE);
	assert(hLowHPPict_ >= 0);


	//データを外部に出せるようになったら変える
	ImageManager::SetImagePos(hPict_[0], { 0,580,0 });
	ImageManager::SetImagePos(hPict_[1], { 0,-580,0 });
	ImageManager::SetImagePos(hPict_[2], { -1410,10,0 });
	ImageManager::SetImagePos(hPict_[3], { 1410,10,0 });
	for (int i : hPict_)
	{
		ImageManager::SetAlpha(i,0);
	}
	ObjectSetter* p = (ObjectSetter*)FindObject("ObjectSetter");
}

void PlayScreen::Draw()
{
	if (((Player*)GetParent())->GetLife() <= LOW_HP)
	{
		ImageManager::Draw(hLowHPPict_);
	}
}
void PlayScreen::SecondDraw()
{
	for (int i : hPict_)
	{
		ImageManager::Draw(i);
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

	if (dir.x >= TILT)
	{
		ImageManager::SetAlpha(hPict_[3], 1);
	}
	if (dir.x <= -TILT)
	{
		ImageManager::SetAlpha(hPict_[2], 1);
	}
	if (dir.y >= TILT)
	{
		ImageManager::SetAlpha(hPict_[0], 1);
	}
	if (dir.y <= -TILT)
	{
		ImageManager::SetAlpha(hPict_[1], 1);
	}
}
