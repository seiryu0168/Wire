#include "MissionUI.h"
#include"Easing.h"

MissionUI::MissionUI(GameObject* parent)
	:GameObject(parent,"MissionUI"),
	pText_(nullptr),
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
	pText_->Load("Enemydestroy!", "Meiryo UI", rect, LEFT_TOP);
}

void MissionUI::Update()
{
	ratio_ += 0.01f;
	if (ratio_ >= 0.5)
	{
		//pText_->SetTextSize(12.0f, 0, 4);
		pText_->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD, 0, 5);
	}
	pText_->SetRatio(Easing::EaseOutQuint(ratio_)*0.4f,0.5f);
}

void MissionUI::Draw()
{
	pText_->Draw();
}

void MissionUI::Release()
{
	SAFE_DELETE(pText_);
}
