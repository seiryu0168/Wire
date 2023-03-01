#include "PlayScreen.h"
#include"Engine/ResourceManager/ImageManager.h"
PlayScreen::PlayScreen()
	:hPict_(-1)
{
	hPict_ = ImageManager::Load("Assets\\ScreenImage.png");
}

PlayScreen::~PlayScreen()
{
}

void PlayScreen::Update()
{
}
