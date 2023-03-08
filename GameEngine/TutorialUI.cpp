#include "TutorialUI.h"
#include"Engine/ResourceManager/ImageManager.h"
#include"Engine/DirectX_11/Input.h"
#include"Engine/SceneManager.h"
TutorialUI::TutorialUI(GameObject* parent)
	:GameObject(parent,"TutorialScene"),
	hPict_(-1)
{
}

TutorialUI::~TutorialUI()
{
}

void TutorialUI::Initialize()
{
	hPict_ = ImageManager::Load("Assets\\ControlImage.png");
	assert(hPict_ >= 0);

	ImageManager::SetImagePos(hPict_, { 1400,0,0 });
}

void TutorialUI::Update()
{
	if (Input::IsPadButtonDown(XINPUT_GAMEPAD_START))
	{
		((SceneManager*)FindObject("SceneManager"))->ChangeScene((int)SCENE_ID::SCENE_ID_TITLE,30);
	}
}

void TutorialUI::Draw()
{
}

void TutorialUI::Release()
{
}
