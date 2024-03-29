#include "TutorialOrder.h"
#include"Engine/DirectX_11/Input.h"
#include"Engine/ResourceManager/ImageManager.h"
#include"Easing.h"
namespace 
{
	static const XMFLOAT2 TEXT_POS = { 0.4f,0.45f };
	static const float DELTA_RATIO = 0.015f;
	static const float DELTA_ALPHA = 0.01f;
	static const std::string BACKGROUND_IMAGE = "Assets\\Image\\TextBackGround.png";
	const std::string BACKBUTTON_SELECT = "Assets\\Image\\BackButton_Selected.png";
	const std::string BACKBUTTON = "Assets\\Image\\BackButton_Default.png";
}

TutorialOrder::TutorialOrder(GameObject* parent)
	:MissionUI(parent,"TutorialOrder"),
	pText_(nullptr),
	ratio_(0),
	hPict_(-1),
	alpha_(1),
	isTutorial_(true)
{
	pText_ = new Text();
	TEXT_RECT rect = { 0,0,500,100 };
	pText_->Load("チュートリアル", "Sitka Text", rect,LEFT_TOP);
	hPict_ = ImageManager::Load(BACKGROUND_IMAGE);

	//バックボタンの画像
	hPictBackButton_ = ImageManager::Load(BACKBUTTON);
	assert(hPictBackButton_ >= 0);
	ImageManager::SetImagePos(hPictBackButton_, { 1500,-900,0 });
}

TutorialOrder::~TutorialOrder()
{
}

void TutorialOrder::Initialize()
{
	FindObject("Player")->SetUpdate(!isTutorial_);
	FindObject("PauseUI")->SetUpdate(!isTutorial_);
	te_.Initialize();
}

void TutorialOrder::Update()
{
	if (isTutorial_ == false)
	{

		ratio_ += DELTA_RATIO;
		//ratio_が1以上になったら
		if (ratio_ >= 1.0f)
		{
			//テキストと背景を徐々に薄くする
			EraseText();
		}
	}
	if (Input::IsPadButtonDown(XINPUT_GAMEPAD_BACK))
		ChangeScreen();

	if (isTutorial_)
		te_.Update();

	//テキスト移動
	textList[0]->SetRatio(Easing::EaseInOutQuint(ratio_) * TEXT_POS.x, TEXT_POS.y);
}

void TutorialOrder::Draw()
{
}

void TutorialOrder::ThirdDraw()
{
	//描画
	textList[0]->Draw();
	ImageManager::Draw(imageList[0]);
	for (auto& i : noticeTextList_)
	{
		i.second->Draw();
	}
	if (isTutorial_)
	{
		te_.Draw();
		ImageManager::Draw(hPictBackButton_);
	}
}

void TutorialOrder::ChangeScreen()
{
	//フラグを変える
	isTutorial_ = !isTutorial_;
	//プレイヤーとポーズのアップデートをオンオフ
	FindObject("Player")->SetUpdate(!isTutorial_);
	FindObject("PauseUI")->SetUpdate(!isTutorial_);
}

void TutorialOrder::EraseText()
{
	//アルファ値減らしていく
	textList[0]->SetTextColor({ 1,1,1,alpha_ });
	ImageManager::SetAlpha(hPict_,alpha_);
	alpha_ -= DELTA_ALPHA;
}

void TutorialOrder::Release()
{
	SAFE_RELEASE(pText_);
}
