#include "ResultUI.h"
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
	static const std::string UI_IMAGE_FILE = "ResultUIData.json";
	static const std::string BUTTON_LIST_NAME = "ResultUIDataList";
	static const float STICK_TILT = 0.7f;
	static const int INFORMATION_OFFSET = 1400;
	static const int INTERVAL = 1;
	static const int TITLE = 0;
	static const int PLAY = 1;
	static const short UP = -1;
	static const short DOWN = 1;
	static const int MAX_BUTTON = 5;
	static const int DELAY = 90;
	static const int MOVE = 500;
	static const int MAX_MOVE_TIME = 11;
	static const XMFLOAT3 BUTTON_FIRST_POS = { -1400.0f,-500.0f,0.0f };
}


ResultUI::ResultUI(GameObject* parent)
	:GameObject(parent, "ResultUI"),
	inputInterval_(INTERVAL),
	moveTime_(0),
	hPictTitle_(-1),
	buttonMove_(0),
	canPushButton_(true),
	isMoveEnd_(true),
	buttonCount_(0)

{
	ReadFile(UI_IMAGE_FILE);
}

ResultUI::~ResultUI()
{
	for (auto& i : buttonList_)
	{
		SAFE_RELEASE(i.buttonText_);
	}
	SAFE_DELETE(fileReader_);
}

void ResultUI::Initialize()
{
	bool isClear=InterSceneData::GetboolData("Result");
	if (isClear)
	{
		hPictResult_ = ImageManager::Load("Assets\\Clear2.png");
	}
	else
	{
		hPictResult_ = ImageManager::Load("Assets\\GameOver2.png");
	}
	ImageManager::SetUIList(hPictResult_);
	assert(hPictResult_ >= 0);
	LoadImageFile();
}

void ResultUI::Update()
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

void ResultUI::MoveButton(float ratio)
{
	int deltaPos = (MOVE / (MAX_MOVE_TIME - 1)) * buttonMove_;
	int delta = MOVE * ratio * buttonMove_;
	for (auto& i : buttonList_)
	{
		//ボタンを移動させる
			XMFLOAT3(i.position_.x,
					 i.position_.y + delta,
					 0);
		i.buttonText_->SetPosition({ i.position_.x - i.buttonText_->GetRect().right,
							   i.position_.y + i.buttonText_->GetRect().bottom + delta });
		i.informationText_->SetPosition({ i.position_.x - i.informationText_->GetRect().right+ INFORMATION_OFFSET,
										  i.position_.y + i.informationText_->GetRect().bottom + delta });
	}
}

bool ResultUI::IsLimit(int buttonNum)
{
	if (buttonNum > 0 || buttonNum < (buttonCount_ - 1))
		return true;
	else
		return false;
}

void ResultUI::Input()
{
	//ボタンの処理
	buttonMove_ = 0;
	//上に移動
	if (Input::GetLStick_Y() >= STICK_TILT)
	{
		buttonMove_ = UP;
		//ボタンが
		if (buttonNum_ > 0)
			uiMode_ = UI_MODE::MODE_MOVE;
	}
	//下に移動
	else if (Input::GetLStick_Y() <= -STICK_TILT)
	{
		buttonMove_ = DOWN;
		//ボタンの番号が最後尾じゃなかったら移動モードに切り替える
		if (buttonNum_ < (buttonCount_ - 1))
			uiMode_ = UI_MODE::MODE_MOVE;
	}
	//ボタンの番号を調整
	buttonNum_ += buttonMove_;
	buttonNum_ = Clamp<float>((float)buttonNum_, 0.0f, (float)(buttonCount_ - 1));

	PushedButton(buttonNum_);
}

void ResultUI::Move()
{
	moveTime_++;
	moveTime_ = min(moveTime_, MAX_MOVE_TIME);
	//動き終わったか
	if (moveTime_ >= MAX_MOVE_TIME)
	{
		uiMode_ = UI_MODE::MODE_INPUT;
		moveTime_ = 0;
		//動かしたセレクト画面の位置を確定させる
		for (auto& i : buttonList_)
		{
			i.position_.y += MOVE * buttonMove_;
		}
		return;
	}
	//セレクト画面を動かす
	MoveButton(Easing::EaseInCubic((float)moveTime_ / (float)(MAX_MOVE_TIME - 1)));
}


void ResultUI::ReadFile(std::string fileName)
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

void ResultUI::ThirdDraw()
{
	for (auto& i : buttonList_)
	{
		i.buttonText_->Draw();
	}

		buttonList_[buttonNum_].informationText_->Draw();
}

void ResultUI::PushedButton(int num)
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

void ResultUI::LoadImageFile()
{
	//ImageManager::SetImagePos()
	//ボタン画像読み込み
	TEXT_RECT rect = { 0,0,500,100 };
	for (auto elem : fileReader_[0][BUTTON_LIST_NAME].items().begin().value())
	{

		//画像の名前を取得
		std::string button = elem[0];
		std::string information = elem[1];

		/////////////////////////////ボタンの用意//////////////////////
		button_ btn;

		//テキスト読み込み
		btn.buttonText_ = new Text;
		btn.buttonText_->Load(button, "Sitka Text", rect, LEFT_TOP);
		btn.informationText_ = new Text;
		btn.informationText_->Load(information, "Sitka Text", rect, LEFT_TOP);


		//位置設定
		btn.position_ = { BUTTON_FIRST_POS.x,
						  BUTTON_FIRST_POS.y+(buttonCount_++) * (float)(-MOVE),
						  0.0f };
		btn.buttonText_->SetPosition({ btn.position_.x - btn.buttonText_->GetRect().right,
									   btn.position_.y + btn.buttonText_->GetRect().bottom });
		btn.informationText_->SetPosition({ btn.position_.x - btn.informationText_->GetRect().right+ INFORMATION_OFFSET,
											btn.position_.y + btn.informationText_->GetRect().bottom });
		//配列に入れる
		buttonList_.push_back(btn);
	}
	hPictButtonFrame_ = ImageManager::Load("Assets\\ButtonFrame.png");
	assert(hPictButtonFrame_ >= 0);

	ImageManager::SetImagePos(hPictButtonFrame_, BUTTON_FIRST_POS);
	ImageManager::SetUIList(hPictButtonFrame_);
}

void ResultUI::Release()
{
}
