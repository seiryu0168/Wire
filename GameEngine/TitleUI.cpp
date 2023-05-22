#include "TitleUI.h"
#include"Engine/DirectX_11/Input.h"
#include"Engine/ResourceManager/ImageManager.h"
#include"Engine/SceneManager.h"
namespace
{
	static const float FLASH_INTERVAL = 1;
}
TitleUI::TitleUI(GameObject* parent)
	:GameObject(parent,"TitleUI"),
	hPictTitle_(-1),
	hPictMessage_(-1),
	flashingTime_(0)
{
}

TitleUI::~TitleUI()
{
}

void TitleUI::Initialize()
{
	hPictTitle_ = ImageManager::Load("Assets\\TitleImage.png");
	assert(hPictTitle_ >= 0);
	hPictMessage_ = ImageManager::Load("Assets\\TitleMessageImage.png");
	assert(hPictTitle_ >= 0);
}

void TitleUI::Update()
{
	flashingTime_++;
	if (Input::IsPadButtonDown(XINPUT_GAMEPAD_A))
	{
		SceneManager* pManager = ((SceneManager*)FindObject("SceneManager"));
		pManager->ChangeScene(SCENE_ID::SCENE_ID_SELECT,180);
	}
	ImageManager::SetAlpha(hPictMessage_,sinf(XMConvertToRadians(flashingTime_ * FLASH_INTERVAL)));
}

void TitleUI::Draw()
{
	ImageManager::Draw(hPictTitle_);
	ImageManager::Draw(hPictMessage_);
}

void TitleUI::Release()
{
}
