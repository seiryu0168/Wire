#include "PlayScreen.h"
#include"Engine/ResourceManager/ImageManager.h"
PlayScreen::PlayScreen()
{
	hPict_[0] = ImageManager::Load("Assets\\ScreenFrameUp2.png");
	assert(hPict_[0] >= 0);
	hPict_[1] = ImageManager::Load("Assets\\ScreenFrameDown2.png");
	assert(hPict_[1] >= 0);
	hPict_[2] = ImageManager::Load("Assets\\ScreenFrameLeft2.png");
	assert(hPict_[2] >= 0);
	hPict_[3] = ImageManager::Load("Assets\\ScreenFrameRight2.png");
	assert(hPict_[3] >= 0);

	ImageManager::SetImagePos(hPict_[0], { 0,580,0 });
	ImageManager::SetImagePos(hPict_[1], { 0,-580,0 });
	ImageManager::SetImagePos(hPict_[2], { -1410,10,0 });
	ImageManager::SetImagePos(hPict_[3], { 1410,10,0 });
}

PlayScreen::~PlayScreen()
{
}

void PlayScreen::Update(XMFLOAT2 dir)
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
