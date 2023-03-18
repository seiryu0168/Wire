#include "MissionUI.h"
#include"Engine/ResourceManager/ImageManager.h"
#include"Easing.h"

MissionUI::MissionUI(GameObject* parent)
	:GameObject(parent,"MissionUI"),
	pText_(nullptr),
	hPict_(-1),
	ratio_(0)
{
}

MissionUI::~MissionUI()
{
}

void MissionUI::Initialize()
{
	pText_ = new Text();
	TEXT_RECT rect = { 0,0,500,100 };
	hPict_=ImageManager::Load("Assets\\TextBackGround.png");
	assert(hPict_ >= 0);

	pText_->Load("敵を倒せ！", "Meiryo UI", rect, LEFT_TOP);
}

void MissionUI::Update()
{
	ratio_ += 0.015f;
	//ratio_が1以上になったら
	if (ratio_ >= 1.0f)
	{
		//アルファ値減らす
		pText_->SetColor({ 1,1,1,max(0,1.0f - (ratio_ - 1.0f)) });
		ImageManager::SetAlpha(hPict_, max(0,1.0f - (ratio_ - 1.0f)));
	}
	//テキスト移動
	pText_->SetRatio(Easing::EaseOutQuint(ratio_)*0.4f,0.45f);
	//ratio_が3以上になったら死ぬ
	if (ratio_ >= 3.0f)
	{
		KillMe();
	}
}

void MissionUI::Draw()
{
	pText_->Draw();
}

void MissionUI::Release()
{
	SAFE_DELETE(pText_);
}
