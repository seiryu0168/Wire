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
	const std::string FILENAME = "TutorialExplanation.json";
	const std::string PATH = "Assets\\IMage\\";
	const std::string FRAME_NAME = "MissionImageFrame.png";
	const std::string BLACKIMAGE_NAME = "Black.png";
	const std::string LARROW[2] = { "LArrow_Default.png","LArrow_Select.png" };
	const std::string RARROW[2] = { "RArrow_Default.png","RArrow_Select.png" };
	const XMFLOAT3 LARROW_POS = { 900.0f,-900.0f,0 };
	const XMFLOAT3 RARROW_POS = { 1200.0f,-900.0f,0 };
	const int MOVE_INTERVAL = 50;
	const float MOVE_RATIO = 90.0f;
	const float TILT = 0.7f;
	const short RIGHT = -1;
	const short LEFT = 1;
	const int MAX_MOVE_TIME = 11;
	const float MARGIN = 40.0f;
	const D2D1_COLOR_F BackColor = { 0,0,0,0.2f };
}
TutorialExplanation::TutorialExplanation()
	:moveTime_(0),
	slideNum_(0),
	mode_(INPUT_MODE::MODE_INPUT),
	hPictTutorialBackGround_(-1),
	hPictFrame_(-1),
	hPictLArrowSelect_(-1),
	hPictLArrow_(-1),
	hPictRArrowSelect_(-1),
	hPictRArrow_(-1)
{
}

TutorialExplanation::~TutorialExplanation()
{
}

void TutorialExplanation::Input()
{
	//入力の処理
	moveDir_ = 0;
	//上に移動
	if (Input::GetLStick_X() >= TILT)
	{
		moveDir_ = RIGHT;
		//スライド番号が最後じゃなければ
		if (slideNum_ < (imageList_.size() - 1))
			mode_ = INPUT_MODE::MODE_MOVE;
	}
	//下に移動
	else if (Input::GetLStick_X() <= -TILT)
	{
		moveDir_ = LEFT;
		//スライド番号が0じゃなかったら
		if (slideNum_ > 0)
			mode_ = INPUT_MODE::MODE_MOVE;
	}
	//スライド番号を調整
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
		//動かしたスライドの位置を確定させる
		for (auto& i : imageList_)
		{
			i.position_.x += MOVE_INTERVAL * moveDir_;
		}
		return;
	}
	//スライドを動かす
	MoveSlide(Easing::EaseInCubic((float)moveTime_ / (float)(MAX_MOVE_TIME - 1)));
}

void TutorialExplanation::Initialize()
{
	if (Load(FILENAME) == false)
		return;
	//チュートリアルの背景画像
	hPictTutorialBackGround_ = ImageManager::Load(PATH + BLACKIMAGE_NAME);
	assert(hPictTutorialBackGround_ >= 0);
	ImageManager::SetAlpha(hPictTutorialBackGround_, 0.2f);
	
	//画像のフレーム画像
	hPictFrame_ = ImageManager::Load(PATH + FRAME_NAME);
	assert(hPictFrame_ >= 0);
	ImageManager::SetImagePos(hPictFrame_, { 0,300,0 });

	hPictLArrow_ = ImageManager::Load(PATH + LARROW[0]);
	assert(hPictLArrow_);
	ImageManager::SetImagePos(hPictLArrow_, LARROW_POS);
	hPictLArrowSelect_ = ImageManager::Load(PATH + LARROW[1]);
	assert(hPictLArrowSelect_);
	ImageManager::SetImagePos(hPictLArrowSelect_, LARROW_POS);
	hPictRArrow_ = ImageManager::Load(PATH + RARROW[0]);
	assert(hPictRArrow_);
	ImageManager::SetImagePos(hPictRArrow_, RARROW_POS);
	hPictRArrowSelect_ = ImageManager::Load(PATH + RARROW[1]);
	assert(hPictRArrowSelect_);
	ImageManager::SetImagePos(hPictRArrowSelect_, RARROW_POS);

	//スライド画像の読み込み
	for (auto &elem : tutorialFile_["ImageList"])
	{
		imageData data;
		std::string fileName = elem[0];
		data.hPict_ = ImageManager::Load(PATH + fileName);
		assert(data.hPict_ >= 0);
		data.position_ = { 50.0f*(float)imageList_.size(),elem[1][1],elem[1][2]};
		ImageManager::SetImagePos(data.hPict_, { data.position_.x * MOVE_RATIO,
												 data.position_.y,
											     data.position_.z });
		imageList_.push_back(data);
	}

	//テキストの情報(CSVファイル)読み込む
	auto textFileData = tutorialFile_["TextList"];
	std::string textFilePath = textFileData[0];
	CsvReader csv_("Assets\\"+textFilePath);
		TEXT_RECT rect = { textFileData[1][0],
						   textFileData[1][1],
						   textFileData[1][2],
						   textFileData[1][3]};

	//テキストリストに追加
	for (int i = 0; i < csv_.GetLines(); i++) 
	{
		Text* pText = new Text();
		std::string str=csv_.GetString(i, 0);
		pText->Load(str, "Arial", rect, ALIGNMENT_TYPE::LEFT_TOP,50);
		pText->SetTransform({ textFileData[2][0]-(rect.right / 2),textFileData[2][1] - (rect.bottom / 2)});
		pText->SetBackColor(BackColor);
		pText->SetMargin(MARGIN);
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
	//動く大きさを求める
	float delta = MOVE_INTERVAL * ratio * moveDir_;
	

	for (auto& i : imageList_)
	{
		//スライドを移動させる
		ImageManager::SetImagePos(i.hPict_,
			XMFLOAT3((i.position_.x + delta)* MOVE_RATIO,
					  i.position_.y,
					 0));
	}
}

bool TutorialExplanation::Load(std::string fileName)
{
	//カレントディレクトリ取得
	WCHAR currentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, currentDir);

	if (SetCurrentDirectory(L"Assets") == ERROR_FILE_NOT_FOUND)
	{
		return false;
	}
	
	//ファイル読み込み
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
	ImageManager::Draw(hPictTutorialBackGround_);
	ImageManager::Draw(hPictFrame_);
	for (int i = 0; i < imageList_.size(); i++)
	{
		ImageManager::Draw(imageList_[i].hPict_);
		if (slideNum_ <= textList_.size())
			textList_[slideNum_]->Draw();
	}

	if (mode_ == INPUT_MODE::MODE_MOVE)
	{
		if (moveDir_ == LEFT)
		{

			ImageManager::Draw(hPictLArrowSelect_);
		}
		else
			ImageManager::Draw(hPictRArrowSelect_);
	}

	ImageManager::Draw(hPictLArrow_);
	ImageManager::Draw(hPictRArrow_);

}