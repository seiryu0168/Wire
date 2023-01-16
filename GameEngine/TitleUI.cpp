#include "TitleUI.h"
#include"Engine/ImageManager.h"

TitleUI::TitleUI(GameObject* parent)
	:GameObject(parent,"TitleUI"),
	hPictPlay_(-1),
	hPictQuit_(-1),
	hPictTitle_(-1)
{

}

TitleUI::~TitleUI()
{
}

void TitleUI::Initialize()
{
	hPictPlay_ = ImageManager::Load("Assets\\PlayButton.jpg");
	assert(hPictPlay_ >= 0);
	ImageManager::SetImagePos(hPictPlay_, { -500,-300,1 });
	hPictTitle_ = ImageManager::Load("Assets\\TitleImage.jpg");
	assert(hPictTitle_ >= 0);
}

void TitleUI::Update()
{
}

void TitleUI::Release()
{
}
