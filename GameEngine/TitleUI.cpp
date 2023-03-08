#include "TitleUI.h"
#include"Engine/ResourceManager/ImageManager.h"
#include"Engine/DirectX_11/Input.h"
#include"Engine/SceneManager.h"

namespace
{
	static const int INTERVAL = 10;
	static const int MAX_BUTTON = 2;
}

TitleUI::TitleUI(GameObject* parent)
	:GameObject(parent,"TitleUI"),
	hPictPlay_(-1),
	hPictTutorial_(-1),
	hPictTitle_(-1)
{

}

TitleUI::~TitleUI()
{
}

void TitleUI::Initialize()
{
	hPictTitle_ = ImageManager::Load("Assets\\TitleImage2.png");
	assert(hPictTitle_ >= 0);
	hPictPlay_ = ImageManager::Load("Assets\\PlayImage.jpg");
	assert(hPictPlay_ >= 0);
	hPictTutorial_ = ImageManager::Load("Assets\\TutorialImage.jpg");
	assert(hPictTutorial_ >= 0);

	//位置設定
	ImageManager::SetImagePos(hPictPlay_, XMFLOAT3(-800, -500, 0));
	ImageManager::SetImagePos(hPictTutorial_, XMFLOAT3(800, -500, 0));
}

void TitleUI::Update()
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
		//プレイボタン
		ImageManager::SetImageSize(hPictPlay_, XMFLOAT3(1.2f, 1.2f, 1.2f));
		ImageManager::ChangeColor(hPictPlay_, XMFLOAT4(1.9f, 1.9f, 1.9f, 1.0f));

		ImageManager::SetImageSize(hPictTutorial_, XMFLOAT3(1, 1, 1));
		ImageManager::ChangeColor(hPictTutorial_, XMFLOAT4(1, 1, 1, 1));

		if (Input::IsPadButtonDown(XINPUT_GAMEPAD_A))
		{
			((SceneManager*)FindObject("SceneManager"))->ChangeScene((int)SCENE_ID::SCENE_ID_PLAY);
		}
		break;
	case 1:
		//チュートリアルボタン
		ImageManager::SetImageSize(hPictTutorial_, XMFLOAT3(1.2f, 1.2f, 1.2f));
		ImageManager::ChangeColor(hPictTutorial_, XMFLOAT4(1.9f, 1.9f, 1.9f, 1.0f));

		ImageManager::SetImageSize(hPictPlay_, XMFLOAT3(1, 1, 1));
		ImageManager::ChangeColor(hPictPlay_, XMFLOAT4(1, 1, 1, 1));
		if (Input::IsPadButtonDown(XINPUT_GAMEPAD_A))
		{
			((SceneManager*)FindObject("SceneManager"))->ChangeScene((int)SCENE_ID::SCENE_ID_TUTORIAL);
		}
		break;

	default:
		break;
	}
	//if (Input::IsPadButtonDown(XINPUT_GAMEPAD_A)||Input::IsKeyDown(DIK_A))
	//{
	//	SceneManager* pManager = ((SceneManager*)FindObject("SceneManager"));
	//	pManager->ChangeScene((int)SCENE_ID::SCENE_ID_PLAY);
	//}
}

void TitleUI::Release()
{
}
