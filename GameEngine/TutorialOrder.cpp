#include "TutorialOrder.h"
#include"Engine/ResourceManager/ImageManager.h"
#include"Easing.h"
namespace 
{
	static const XMFLOAT2 TEXT_POS = { 0.4f,0.45f };
	static const float DELTA_RATIO = 0.015f;
	static const float DELTA_ALPHA = 0.01f;
}
TutorialOrder::TutorialOrder(GameObject* parent)
	:MissionUI(parent,"TutorialOrder"),
	pText_(nullptr),
	ratio_(0),
	hPict_(-1),
	alpha_(1)
{
	pText_ = new Text();
	TEXT_RECT rect = { 0,0,500,100 };
	pText_->Load("チュートリアル", "Sitka Text", rect,LEFT_TOP);
	hPict_ = ImageManager::Load("Assets\\TextBackGround.png");
}

TutorialOrder::~TutorialOrder()
{
}

void TutorialOrder::Initialize()
{
}

void TutorialOrder::Update()
{
	ratio_ += DELTA_RATIO;
	//ratio_が1以上になったら
	if (ratio_ >= 1.0f)
	{
		//テキストと背景を徐々に薄くする
		EraseText();
	}

	//テキスト移動
	textList[0]->SetRatio(Easing::EaseInOutQuint(ratio_) * TEXT_POS.x, TEXT_POS.y);
}

void TutorialOrder::Draw()
{
	textList[0]->Draw();
	ImageManager::Draw(imageList[0]);
}

void TutorialOrder::EraseText()
{
	textList[0]->SetColor({ 1,1,1,alpha_ });
	ImageManager::SetAlpha(hPict_,alpha_);
	alpha_ -= DELTA_ALPHA;
}

void TutorialOrder::Release()
{
	SAFE_RELEASE(pText_);
}
