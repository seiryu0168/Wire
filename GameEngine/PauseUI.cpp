#include "PauseUI.h"
#include"Engine/ResourceManager/ImageManager.h"
#include"InterSceneData.h"
#include"Engine/DirectX_11/Input.h"
#include"Engine/SceneManager.h"
#include"Engine/ResourceManager/Text.h"
#include<fstream>
#include<sstream>
#include"Easing.h"

namespace
{
	static const std::string UI_DATA_FILE = "PauseUIData.json";
	static const std::string BUTTON_LIST_NAME = "PauseUIDataList";
	static const float STICK_TILT = 0.7f;
	static const int INFORMATION_OFFSET = 1400;
	static const int INTERVAL = 1;
	static const int TITLE = 0;
	static const int PLAY = 1;
	static const short LEFT = -1;
	static const short RIGHT = 1;
	static const int MAX_BUTTON = 5;
	static const int DELAY = 90;
	static const int MOVE = 1200;
	static const int MAX_MOVE_TIME = 11;
	static const XMFLOAT3 BUTTON_FIRST_POS = { -600.0f,-400.0f,0.0f };
}


PauseUI::PauseUI(GameObject* parent)
	:GameObject(parent, "PauseUI"),
	inputInterval_(INTERVAL),
	moveTime_(0),
	hPictBackGround_(-1),
	hPictButtonFrame_(-1),
	buttonMove_(0),
	buttonCount_(0),
	framePos_(BUTTON_FIRST_POS)

{
	ReadFile(UI_DATA_FILE);
}

PauseUI::~PauseUI()
{
	for (auto& i : buttonList_)
	{
		SAFE_RELEASE(i.buttonText_);	
	}
	SAFE_DELETE(fileReader_);
}

void PauseUI::Initialize()
{
	LoadImageFile();
}

void PauseUI::Update()
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
}

void PauseUI::MoveButton(float ratio)
{
	int deltaPos = (MOVE / (MAX_MOVE_TIME - 1)) * buttonMove_;
	int delta = MOVE * ratio * buttonMove_;
	XMFLOAT3 pos = framePos_;
	pos.x += delta;
	ImageManager::SetImagePos(hPictButtonFrame_, pos);
}

bool PauseUI::IsLimit(int buttonNum)
{
	if (buttonNum > 0 || buttonNum < (buttonCount_ - 1))
		return true;
	else
		return false;
}

void PauseUI::Input()
{
	//ボタンの処理
	buttonMove_ = 0;
	//右に移動
	if (Input::GetLStick_X() >= STICK_TILT)
	{
		buttonMove_ = RIGHT;
		//ボタンが最後尾じゃなかったら移動モードに切り替える
		if (buttonNum_ < (buttonCount_ - 1))
			uiMode_ = UI_MODE::MODE_MOVE;
	}
	//左に移動
	else if (Input::GetLStick_X() <= -STICK_TILT)
	{
		buttonMove_ = LEFT;
		//ボタンの番号が最初じゃなかったら移動モードに切り替え
		if (buttonNum_ >0)
			uiMode_ = UI_MODE::MODE_MOVE;
	}
	//ボタンの番号を調整
	buttonNum_ += buttonMove_;
	buttonNum_ = Clamp<float>((float)buttonNum_, 0.0f, (float)(buttonCount_ - 1));

	PushedButton(buttonNum_);
}

void PauseUI::Move()
{
	moveTime_++;
	moveTime_ = min(moveTime_, MAX_MOVE_TIME);
	//動き終わったか
	if (moveTime_ >= MAX_MOVE_TIME)
	{
		uiMode_ = UI_MODE::MODE_INPUT;
		moveTime_ = 0;
		//動かしたセレクト画面の位置を確定させる
		framePos_.x += MOVE * buttonMove_;
		return;
	}
	//セレクト画面を動かす
	MoveButton(Easing::EaseInCubic((float)moveTime_ / (float)(MAX_MOVE_TIME - 1)));
}


void PauseUI::ReadFile(std::string fileName)
{
	//カレントディレクトリ取得
	WCHAR currentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, currentDir);

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

void PauseUI::ThirdDraw()
{
	ImageManager::Draw(hPictBackGround_);
	for (auto& i : buttonList_)
	{
		i.buttonText_->Draw();
	}
}

void PauseUI::PushedButton(int num)
{
	if (Input::IsPadButtonDown(XINPUT_GAMEPAD_A))
	{
		switch (num)
		{
		case TITLE:
			((SceneManager*)FindObject("SceneManager"))->ChangeScene(SCENE_ID::SCENE_ID_SELECT, DELAY);
			break;
		case PLAY:
			((SceneManager*)FindObject("SceneManager"))->ChangeScene(SCENE_ID::SCENE_ID_PLAY, DELAY);
			break;
		}
	}
}

void PauseUI::LoadImageFile()
{
	hPictBackGround_ = ImageManager::Load("Assets\\LowHPScreen.png");
	assert(hPictBackGround_ >= 0);
	//ボタン画像読み込み
	TEXT_RECT rect = { 0,0,500,100 };
	for (auto elem : fileReader_[0][BUTTON_LIST_NAME].items().begin().value())
	{
		buttonCount_++;
		//画像の名前を取得
		std::string button = elem[0];
		XMFLOAT3 pos = { elem[1][0],elem[1][1] ,elem[1][2] };

		/////////////////////////////ボタンの用意//////////////////////
		button_ btn;

		//テキスト読み込み
		btn.buttonText_ = new Text;
		btn.buttonText_->Load(button, "Sitka Text", rect, LEFT_TOP);


		//位置設定
		btn.position_ = pos;
		btn.buttonText_->SetPosition({ btn.position_.x - btn.buttonText_->GetRect().right,
									   btn.position_.y + btn.buttonText_->GetRect().bottom });
		//配列に入れる
		buttonList_.push_back(btn);
	}
	hPictButtonFrame_ = ImageManager::Load("Assets\\ButtonFrame.png");
	assert(hPictButtonFrame_ >= 0);

	ImageManager::SetImagePos(hPictButtonFrame_, BUTTON_FIRST_POS);
	ImageManager::SetUIList(hPictButtonFrame_);
}

void PauseUI::Release()
{
}
