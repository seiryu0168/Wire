#include "TitleUI.h"
#include"Engine/DirectX_11/Input.h"
#include"Engine/ResourceManager/ImageManager.h"
#include"Engine/SceneManager.h"
namespace
{
	static const float FLASH_INTERVAL = 2.5f;
	static const XMFLOAT3 MESSAGE_POS = { 0.0f,-300.0f,0.0f };
}
TitleUI::TitleUI(GameObject* parent)
	:GameObject(parent,"TitleUI"),
	hPictTitle_(-1),
	hPictMessage_(-1),
	flashingTime_(0),
	isButtonPush_(false)
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
	ImageManager::SetImagePos(hPictMessage_, MESSAGE_POS);
}

void TitleUI::Update()
{
	flashingTime_++;
	if (Input::IsPadButtonDown(XINPUT_GAMEPAD_A)&& isButtonPush_ ==false)
	{
		SceneManager* pManager = ((SceneManager*)FindObject("SceneManager"));
		pManager->ChangeScene(SCENE_ID::SCENE_ID_SELECT,180);
		isButtonPush_ = true;
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
