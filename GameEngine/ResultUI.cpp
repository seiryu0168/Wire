#include "ResultUI.h"
#include"Engine/ResourceManager/ImageManager.h"
#include"Engine/SceneManager.h"
#include"Engine/DirectX_11/Input.h"
#include"InterSceneData.h"

namespace
{
	static const int INTERVAL = 10;
	static const int MAX_BUTTON=2;
	static const int DELAY = 120;
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
	//クリアかゲームオーバーかで表示する画像を変える
	if (InterSceneData::GetboolData("Result"))
	hPict_ = ImageManager::Load("Assets\\Clear2.png");

	else if (InterSceneData::GetboolData("Result")==false)
		hPict_ = ImageManager::Load("Assets\\GameOver2.png");

	//ボタンの画像ロード
	hPictTitle_ = ImageManager::Load("Assets\\ReturnTitle.png");
	assert(hPictTitle_ >= 0);
	hPictTitle2_ = ImageManager::Load("Assets\\ReturnTitle2.png");
	assert(hPictTitle2_ >= 0);
	hPictRetry_ = ImageManager::Load("Assets\\Retry.png");
	assert(hPictRetry_ >= 0);
	hPictRetry2_ = ImageManager::Load("Assets\\Retry2.png");
	assert(hPictRetry2_ >= 0);
	hPictButtonFrame_ = ImageManager::Load("Assets\\ButtonFrame.png");
	//位置設定
	ImageManager::SetImagePos(hPictTitle_, XMFLOAT3(-800, -500, 0));
	ImageManager::SetImagePos(hPictTitle2_, XMFLOAT3(-800, -500, 0));
	ImageManager::SetImagePos(hPictRetry_, XMFLOAT3(800, -500, 0));
	ImageManager::SetImagePos(hPictRetry2_, XMFLOAT3(800, -500, 0));
	ImageManager::SetImagePos(hPictButtonFrame_, XMFLOAT3(-800, -500, 0));
	ImageManager::SetAlpha(hPictRetry2_, 0);
	ImageManager::SetAlpha(hPictTitle2_, 0);
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
		//ImageManager::SetImageSize(hPictTitle_, XMFLOAT3(1.2f, 1.2f, 1.2f));
		ImageManager::ChangeColor(hPictTitle_, XMFLOAT4(1.9f, 1.9f, 1.9f, 1.0f));
		ImageManager::SetAlpha(hPictTitle2_, 1);
		ImageManager::SetAlpha(hPictRetry2_, 0);
		ImageManager::SetImagePos(hPictButtonFrame_, XMFLOAT3(-800, -500, 0));
		ImageManager::SetImageSize(hPictRetry_, XMFLOAT3(1, 1, 1));
		ImageManager::ChangeColor(hPictRetry_, XMFLOAT4(1, 1, 1, 1));

		if (Input::IsPadButtonDown(XINPUT_GAMEPAD_A))
		{
			((SceneManager*)FindObject("SceneManager"))->ChangeScene(SCENE_ID::SCENE_ID_TITLE, DELAY);
		}
		break;
	case 1:
		//リトライボタン
		//ImageManager::SetImageSize(hPictRetry_, XMFLOAT3(1.2f, 1.2f, 1.2f));
		ImageManager::ChangeColor(hPictRetry_, XMFLOAT4(1.9f, 1.9f, 1.9f, 1.0f));
		ImageManager::SetImagePos(hPictButtonFrame_, XMFLOAT3(800, -500, 0));
		ImageManager::SetAlpha(hPictTitle2_, 0);
		ImageManager::SetAlpha(hPictRetry2_, 1);
		
		ImageManager::SetImageSize(hPictTitle_, XMFLOAT3(1, 1, 1));
		ImageManager::ChangeColor(hPictTitle_, XMFLOAT4(1, 1, 1, 1));
		if (Input::IsPadButtonDown(XINPUT_GAMEPAD_A))
		{
			((SceneManager*)FindObject("SceneManager"))->ChangeScene(SCENE_ID::SCENE_ID_PLAY, DELAY);
		}
		break;

	default:
		break;
	}
}

void ResultUI::Release()
{
}
