#include "ResultUI.h"
#include"Engine/ImageManager.h"
#include"Engine/SceneManager.h"
#include"Engine/Input.h"
#include"InterSceneData.h"

namespace
{
	static const int INTERVAL = 10;
	static const int MAX_BUTTON=2;
}

ResultUI::ResultUI(GameObject* parent)
	:GameObject(parent,"ResultUI"),
	hPict_(-1),
	hPictTitle_(-1),
	hPictRetry_(-1)
{
}

ResultUI::~ResultUI()
{
}

void ResultUI::Initialize()
{
	if (InterSceneData::GetboolData("Result"))
	hPict_ = ImageManager::Load("Assets\\Clear.png");

	else if (InterSceneData::GetboolData("Result")==false)
		hPict_ = ImageManager::Load("Assets\\GameOver.png");

	hPictTitle_ = ImageManager::Load("Assets\\ReturnTitle.png");
	assert(hPictTitle_ >= 0);
	hPictRetry_ = ImageManager::Load("Assets\\ReStart.png");
	assert(hPictRetry_ >= 0);

	ImageManager::SetImagePos(hPictTitle_, XMFLOAT3(-800, -500, 0));
	ImageManager::SetImagePos(hPictRetry_, XMFLOAT3(800, -500, 0));
}

void ResultUI::Update()
{
	//入力の間隔を広くする
	inputInterval_++;
	if (inputInterval_ > INTERVAL)
	{
		if (Input::GetLStick_X() >= 0.7f)
		{
			buttonNum_++;
		}
		else if (Input::GetLStick_X() <= -0.7f)
		{
			buttonNum_--;
		}
		inputInterval_ = 0;
	}
	int num = abs(buttonNum_ % MAX_BUTTON);
	switch (num)
	{
	case 0:
		//タイトルシーンボタン
		ImageManager::SetImageSize(hPictTitle_, XMFLOAT3(1.2f, 1.2f, 1.2f));
		ImageManager::ChangeColor(hPictTitle_, XMFLOAT4(1.9f, 1.9f, 1.9f, 1.0f));

		ImageManager::SetImageSize(hPictRetry_, XMFLOAT3(1, 1, 1));
		ImageManager::ChangeColor(hPictRetry_, XMFLOAT4(1, 1, 1, 1));

		if (Input::IsPadButtonDown(XINPUT_GAMEPAD_A))
		{
			((SceneManager*)FindObject("SceneManager"))->ChangeScene((int)SCENE_ID::SCENE_ID_TITLE);
		}
		break;
	case 1:
		//リトライボタン
		ImageManager::SetImageSize(hPictRetry_, XMFLOAT3(1.2f, 1.2f, 1.2f));
		ImageManager::ChangeColor(hPictRetry_, XMFLOAT4(1.9f, 1.9f, 1.9f, 1.0f));

		ImageManager::SetImageSize(hPictTitle_, XMFLOAT3(1, 1, 1));
		ImageManager::ChangeColor(hPictTitle_, XMFLOAT4(1, 1, 1, 1));
		if (Input::IsPadButtonDown(XINPUT_GAMEPAD_A))
		{
			((SceneManager*)FindObject("SceneManager"))->ChangeScene((int)SCENE_ID::SCENE_ID_PLAY);
		}
		break;

	default:
		break;
	}
}

void ResultUI::Release()
{
}
