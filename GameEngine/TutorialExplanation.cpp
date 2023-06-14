#include "TutorialExplanation.h"
#include"Engine/ResourceManager/ImageManager.h"
#include"Engine/ResourceManager/Text.h"
#include"Engine/ResourceManager/CsvReader.h"
#include"Engine/DirectX_11/Input.h"
#include"Easing.h"
#include<d3d11.h>
#include<fstream>
#include<sstream>
namespace 
{
	static const std::string FILENAME = "TutorialExplanation.json";
	static const std::string PATH = "Assets\\IMage\\";
	static const int MOVE_INTERVAL = 1000;
	static const float TILT = 0.7f;
	static const short RIGHT = -1;
	static const short LEFT = 1;
	static const int MAX_MOVE_TIME = 11;
}
void TutorialExplanation::Input()
{
	//ボタンの処理
	moveDir_ = 0;
	//上に移動
	if (Input::GetLStick_X() >= TILT)
	{
		moveDir_ = RIGHT;
		//ボタンが
		if (slideNum_ < (imageList_.size() - 1))
			mode_ = INPUT_MODE::MODE_MOVE;
	}
	//下に移動
	else if (Input::GetLStick_X() <= -TILT)
	{
		moveDir_ = LEFT;
		if (slideNum_ > 0)
			mode_ = INPUT_MODE::MODE_MOVE;
		//ボタンの番号が最後尾じゃなかったら移動モードに切り替える
	}
	//ボタンの番号を調整
	slideNum_ -= moveDir_;
	slideNum_ = Clamp<float>(slideNum_, 0, imageList_.size() - 1);
}
void TutorialExplanation::Move()
{
	moveTime_++;
	moveTime_ = min(moveTime_, MAX_MOVE_TIME);
	//動き終わったか
	if (moveTime_ >= MAX_MOVE_TIME)
	{
		mode_ = INPUT_MODE::MODE_INPUT;
		moveTime_ = 0;
		//動かしたセレクト画面の位置を確定させる
		for (auto& i : imageList_)
		{
			i.position_.x += MOVE_INTERVAL * moveDir_;
		}
		return;
	}
	//セレクト画面を動かす
	MoveSlide(Easing::EaseInCubic((float)moveTime_ / (float)(MAX_MOVE_TIME - 1)));
}
TutorialExplanation::TutorialExplanation()
	:moveTime_(0),
	mode_(INPUT_MODE::MODE_INPUT)
{
}

TutorialExplanation::~TutorialExplanation()
{
}

void TutorialExplanation::Initialize()
{
	if (Load(FILENAME) == false)
		return;


	for (auto &elem : tutorialFile_["ImageList"])
	{
		imageData data;
		std::string fileName = elem[0];
		data.hPict_ = ImageManager::Load(PATH + fileName);
		assert(data.hPict_ >= 0);
		data.position_ = { elem[1][0],elem[1][1],elem[1][2] };
		ImageManager::SetImagePos(data.hPict_, data.position_);
		imageList_.push_back(data);
	}
	auto textFileData = tutorialFile_["TextList"];
	std::string textFilePath = textFileData[0];
	CsvReader csv_("Assets\\"+textFilePath);
		TEXT_RECT rect = { textFileData[1][0],
						   textFileData[1][1],
						   textFileData[1][2],
						   textFileData[1][3]};

	for (int i = 0; i < csv_.GetLines(); i++) 
	{
		Text* pText = new Text();
		std::string str=csv_.GetString(i, 0);
		pText->Load(str, "Arial", rect, ALIGNMENT_TYPE::LEFT_TOP,50);
		pText->SetTransform({ textFileData[2][0]-(rect.right / 2),textFileData[2][1] - (rect.bottom / 2)});
		textList_.push_back(pText);
	}
}

void TutorialExplanation::Update()
{
	switch (mode_)
	{
	case INPUT_MODE::MODE_INPUT:
		Input();
		break;
	case INPUT_MODE::MODE_MOVE:
		Move();
		break;
	default:
		break;
	}
}

void TutorialExplanation::MoveSlide(float ratio)
{
	float delta = MOVE_INTERVAL * ratio * moveDir_;
	for (auto& i : imageList_)
	{
		//画像を移動させる
		ImageManager::SetImagePos(i.hPict_,
			XMFLOAT3(i.position_.x + delta,
					 i.position_.y,
					 0));
	}
}

//Text TutorialExplanation::CreateText(std::string str)
//{
//	return "";
//}

bool TutorialExplanation::Load(std::string fileName)
{
	//カレントディレクトリ取得
	WCHAR currentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, currentDir);

	if (SetCurrentDirectory(L"Assets") == ERROR_FILE_NOT_FOUND)
	{
		return false;
	}
	std::ifstream fileReader;
	fileReader.open(fileName.c_str());
	if (fileReader.good() == false)
	{
		return false;
	}
	tutorialFile_ = json::parse(fileReader);
	fileReader.close();
	SetCurrentDirectory(currentDir);
	return true;
}

void TutorialExplanation::Draw()
{
	for (int i=0;i<imageList_.size();i++)
	{
		ImageManager::Draw(imageList_[i].hPict_);
		textList_[0]->Draw();
		
	}
}
