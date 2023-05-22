#include "TitleUI.h"
#include"Engine/DirectX_11/Input.h"
#include"Engine/ResourceManager/ImageManager.h"
#include"Engine/SceneManager.h"
TitleUI::TitleUI(GameObject* parent)
	:GameObject(parent,"TitleUI")
{
}

TitleUI::~TitleUI()
{
}

void TitleUI::Initialize()
{
	hPictTitle_ = ImageManager::Load("Assets\\TitleImage.png");
	assert(hPictTitle_ >= 0);
}

void TitleUI::Update()
{
	if (Input::IsPadButtonDown(XINPUT_GAMEPAD_A))
	{
		SceneManager* pManager = ((SceneManager*)FindObject("SceneManager"));
		pManager->ChangeScene(SCENE_ID::SCENE_ID_SELECT,180);
	}
}

void TitleUI::Draw()
{
	ImageManager::Draw(hPictTitle_);
}

void TitleUI::Release()
{
}
