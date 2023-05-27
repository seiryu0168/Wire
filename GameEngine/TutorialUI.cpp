#include "TutorialUI.h"
#include"Engine/ResourceManager/ImageManager.h"
#include"Engine/DirectX_11/Input.h"
#include"InterSceneData.h"
#include"Engine/SceneManager.h"

namespace
{
	static const int DELAY = 120;
	static const std::string CONTROL_IMAGE = "Assets\\Image\\ControlImage.png";
}
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
	hPict_ = ImageManager::Load(CONTROL_IMAGE);
	assert(hPict_ >= 0);

	ImageManager::SetImagePos(hPict_, { 1400,0,0 });
}

void TutorialUI::Update()
{
	if (Input::IsPadButtonDown(XINPUT_GAMEPAD_START))
	{
		InterSceneData::SetData("StageNum", nullptr, 0,nullptr,nullptr);
		((SceneManager*)FindObject("SceneManager"))->ChangeScene(SCENE_ID::SCENE_ID_TITLE, DELAY);
	}
}

void TutorialUI::Draw()
{
}

void TutorialUI::Release()
{
}
