#include "TitleUI.h"
#include"Engine/ResourceManager/ImageManager.h"
#include"InterSceneData.h"
#include"Engine/DirectX_11/Input.h"
#include"Engine/SceneManager.h"

namespace
{
	static const int INTERVAL = 10;
	static const int MAX_BUTTON = 2;
	static const int DELAY = 90;
	static const XMFLOAT2 BUTTON_FRAME_POS = { 800.0f,-500.0f };
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
	hPictPlay_ = ImageManager::Load("Assets\\PlayImage.png");
	assert(hPictPlay_ >= 0);
	hPictPlay2_ = ImageManager::Load("Assets\\PlayImage2.png");
	assert(hPictPlay2_ >= 0);
	hPictTutorial_ = ImageManager::Load("Assets\\TutorialImage.png");
	assert(hPictTutorial_ >= 0);
	hPictTutorial2_ = ImageManager::Load("Assets\\TutorialImage2.png");
	assert(hPictTutorial2_ >= 0);
	hPictButtonFrame_ = ImageManager::Load("Assets\\ButtonFrame.png");
	assert(hPictButtonFrame_ >= 0);

	//位置設定
	ImageManager::SetImagePos(hPictPlay_, XMFLOAT3(-800, -500, 0));
	ImageManager::SetImagePos(hPictPlay2_, XMFLOAT3(-800, -500, 0));
	ImageManager::SetImagePos(hPictButtonFrame_, XMFLOAT3(-800, -500, 0));
	ImageManager::SetImagePos(hPictTutorial_, XMFLOAT3(800, -500, 0));
	ImageManager::SetImagePos(hPictTutorial2_, XMFLOAT3(800, -500, 0));
	ImageManager::SetAlpha(hPictPlay2_, 0);
	
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
		//チュートリアルボタン
		ImageManager::SetImagePos(hPictButtonFrame_, XMFLOAT3(BUTTON_FRAME_POS.x,BUTTON_FRAME_POS.y, 0));
		
		ImageManager::SetAlpha(hPictTutorial2_, 1);
		ImageManager::SetAlpha(hPictPlay2_, 0);

		ImageManager::ChangeColor(hPictPlay_, XMFLOAT4(1, 1, 1, 1));
		if (Input::IsPadButtonDown(XINPUT_GAMEPAD_A))
		{
			InterSceneData::SetData("StageNum", nullptr, &num, nullptr, nullptr);
			((SceneManager*)FindObject("SceneManager"))->ChangeScene(SCENE_ID::SCENE_ID_TUTORIAL, DELAY);
		}
		break;
	case 1:
		//プレイボタン

		ImageManager::SetImagePos(hPictButtonFrame_, XMFLOAT3(-BUTTON_FRAME_POS.x, BUTTON_FRAME_POS.y, 0));
		ImageManager::SetAlpha(hPictTutorial2_, 0);

		ImageManager::ChangeColor(hPictTutorial_, XMFLOAT4(1, 1, 1, 1));
		ImageManager::SetAlpha(hPictPlay2_, 1);
		
		if (Input::IsPadButtonDown(XINPUT_GAMEPAD_A))
		{
			InterSceneData::SetData("StageNum", nullptr,&num,nullptr,nullptr);
			((SceneManager*)FindObject("SceneManager"))->ChangeScene(SCENE_ID::SCENE_ID_PLAY, DELAY);
		}
		break;

	default:
		break;
	}
}

void TitleUI::Release()
{
}
