#include "TitleUI.h"
#include"Engine/ImageManager.h"
#include"Engine/Input.h"
#include"Engine/SceneManager.h"

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
	if (Input::IsPadButtonDown(XINPUT_GAMEPAD_A))
	{
		SceneManager* pManager = ((SceneManager*)FindObject("SceneManager"));
		pManager->ChangeScene((int)SCENE_ID::SCENE_ID_PLAY);
	}
}

void TitleUI::Release()
{
}
