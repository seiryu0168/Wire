#include "TitleUI.h"
#include"Engine/ResourceManager/ImageManager.h"
#include"InterSceneData.h"
#include"Engine/DirectX_11/Input.h"
#include"Engine/SceneManager.h"
#include<fstream>
#include<sstream>
#include"Easing.h"

namespace
{
	static const std::string UI_IMAGE_FILE = "StageSelectUIImage.json";
	static const std::string BUTTON_LIST_NAME = "StageSelectUIImageList";
	static const int INTERVAL = 1;
	static const int MAX_BUTTON = 5;
	static const int DELAY = 90;
	static const int MOVE = 500;
	static const int MAX_MOVE_TIME = 11;
	static const XMFLOAT3 BUTTON_FRAME_POS = { -1400.0f,0.0f,0.0f };
}


TitleUI::TitleUI(GameObject* parent)
	:GameObject(parent, "TitleUI"),
	inputInterval_(INTERVAL),
	moveTime_(0),
	hPictTitle_(-1),
	buttonMove_(0),
	canPushButton_(true),
	isMoveEnd_(true)

{

	ReadFile(UI_IMAGE_FILE);
}

TitleUI::~TitleUI()
{
}

void TitleUI::Initialize()
{
	LoadImageFile();
}

void TitleUI::Update()
{
	switch (uiMode_)
	{
	case UI_MODE::MODE_INPUT:
		Input();
		break;
	case UI_MODE::MODE_MOVE:
		Move();
		break;
	}

	//switch (num)
	//{
	//case 0:
	//	//チュートリアルボタン
	//	ImageManager::SetImagePos(hPictButtonFrame_, XMFLOAT3(BUTTON_FRAME_POS.x,BUTTON_FRAME_POS.y, 0));
	//	
	//	ImageManager::SetAlpha(hPictTutorial2_, 1);
	//	ImageManager::SetAlpha(hPictPlay2_, 0);
	//
	//	ImageManager::ChangeColor(hPictPlay_, XMFLOAT4(1, 1, 1, 1));
	//	if (Input::IsPadButtonDown(XINPUT_GAMEPAD_A))
	//	{
	//		InterSceneData::SetData("StageNum", nullptr, &num, nullptr, nullptr);
	//		((SceneManager*)FindObject("SceneManager"))->ChangeScene(SCENE_ID::SCENE_ID_TUTORIAL, DELAY);
	//	}
	//	break;
	//case 1:
	//	//プレイボタン
	//
	//	ImageManager::SetImagePos(hPictButtonFrame_, XMFLOAT3(-BUTTON_FRAME_POS.x, BUTTON_FRAME_POS.y, 0));
	//	ImageManager::SetAlpha(hPictTutorial2_, 0);
	//
	//	ImageManager::ChangeColor(hPictTutorial_, XMFLOAT4(1, 1, 1, 1));
	//	ImageManager::SetAlpha(hPictPlay2_, 1);
	//	
	//	if (Input::IsPadButtonDown(XINPUT_GAMEPAD_A))
	//	{
	//		InterSceneData::SetData("StageNum", nullptr,&num,nullptr,nullptr);
	//		((SceneManager*)FindObject("SceneManager"))->ChangeScene(SCENE_ID::SCENE_ID_PLAY, DELAY);
	//	}
	//	break;
	//case 2:
	//	if (Input::IsPadButtonDown(XINPUT_GAMEPAD_A))
	//	{
	//		InterSceneData::SetData("StageNum", nullptr, &num, nullptr, nullptr);
	//		((SceneManager*)FindObject("SceneManager"))->ChangeScene(SCENE_ID::SCENE_ID_PLAY, DELAY);
	//	}
	//	break;
	//case 3:
	//	if (Input::IsPadButtonDown(XINPUT_GAMEPAD_A))
	//	{
	//		InterSceneData::SetData("StageNum", nullptr, &num, nullptr, nullptr);
	//		((SceneManager*)FindObject("SceneManager"))->ChangeScene(SCENE_ID::SCENE_ID_PLAY, DELAY);
	//	}
	//	break;
	//
	//default:
	//	break;
	//}
}

void TitleUI::MoveButton(int num)
{
	int deltaPos = (MOVE / (MAX_MOVE_TIME-1)) * buttonMove_;

	for (int i = 0; i < MAX_BUTTON; i++)
	{
		buttonList_[i].position_.y += deltaPos;
		ImageManager::SetImagePos(buttonList_[i].hButtonPict_,
			XMFLOAT3(buttonList_[i].position_.x,
				buttonList_[i].position_.y,
				0));
		ImageManager::SetImagePos(buttonList_[i].hMissionPict_,
			XMFLOAT3(buttonList_[i].position_.x+1000,
				buttonList_[i].position_.y,
				0));
	}

	//ImageManager::SetImagePos(hPictButtonFrame_, {-800,0})
	//ImageManager::SetImagePos(hPictPlay2_, XMFLOAT3(-800, pos-0, 0));
	//ImageManager::SetImagePos(hPictButtonFrame_, XMFLOAT3(-800, -500, 0));
	//ImageManager::SetImagePos(hPictTutorial_, XMFLOAT3(-800, pos-400, 0));
	//ImageManager::SetImagePos(hPictTutorial2_, XMFLOAT3(-800, pos-400, 0));

}

bool TitleUI::IsLimit(int buttonNum)
{
	if(buttonNum > 0 || buttonNum < (MAX_BUTTON - 1))
	return true;
	else
		return false;
}

void TitleUI::Input()
{
		buttonMove_ = 0;
		if (Input::GetLStick_Y() >= 0.7f)
		{
			buttonMove_ = -1;
			if(buttonNum_>0)
			uiMode_ = UI_MODE::MODE_MOVE;
			//buttonMoveSpeed_ = -(MOVE / INTERVAL);
		}
		else if (Input::GetLStick_Y() <= -0.7f)
		{
			buttonMove_ = 1;
			//buttonMoveSpeed_ = MOVE / INTERVAL;
			if(buttonNum_<(MAX_BUTTON-1))
			uiMode_ = UI_MODE::MODE_MOVE;
		}

		buttonNum_ += buttonMove_;
		buttonNum_ = Clamp(buttonNum_, 0, MAX_BUTTON-1);
}

void TitleUI::Move()
{
	moveTime_++;
	moveTime_ = min(moveTime_, MAX_MOVE_TIME);
	if (moveTime_ >= MAX_MOVE_TIME)
	{
		uiMode_=UI_MODE::MODE_INPUT;
		moveTime_ = 0;
		return;
	}

		MoveButton(buttonNum_);
}


void TitleUI::ReadFile(std::string fileName)
{
	//カレントディレクトリ取得
	WCHAR currentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, currentDir);
	
	WCHAR path[FILENAME_MAX];
	
	if (SetCurrentDirectory(L"Assets") == ERROR_FILE_NOT_FOUND)
	{
		return;
	}
	std::ifstream fRead;
	fRead.open(fileName.c_str());
	if (fRead.good())
	{
		fileReader_ = new json;
		*fileReader_ = json::parse(fRead);
	}
	fRead.close();
	SetCurrentDirectory(currentDir);
}

void TitleUI::LoadImageFile()
{


	//タイトル画像読み込み
	hPictTitle_ = ImageManager::Load("Assets\\TitleImage2.png");
	assert(hPictTitle_ >= 0);

	//ボタン画像読み込み
	int i = 0;
	for (auto elem :fileReader_[0][BUTTON_LIST_NAME].items().begin().value())
	{
		//画像の名前を取得
		std::string buttonImageName = elem[0];
		std::string missionName = elem[1];
		//画像読み込み
		button_ btn;
		btn.hButtonPict_ = ImageManager::Load("Assets\\"+buttonImageName);
		assert(btn.hButtonPict_ >= 0);
		btn.hMissionPict_ = ImageManager::Load("Assets\\"+missionName);
		assert(btn.hMissionPict_ >= 0);
		//位置設定
		btn.position_ = { -1400.0f,(i++) * (float)(-MOVE),0.0f };
		ImageManager::SetImagePos(btn.hButtonPict_, btn.position_);
		//
		buttonList_.push_back(btn);
	}
	hPictButtonFrame_ = ImageManager::Load("Assets\\ButtonFrame.png");
	assert(hPictButtonFrame_ >= 0);

	ImageManager::SetImagePos(hPictButtonFrame_, BUTTON_FRAME_POS);
}

void TitleUI::Release()
{
}
