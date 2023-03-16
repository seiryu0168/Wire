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
	pText_->Load("All Enemy Destroy!", "Meiryo UI", rect, LEFT_TOP);
}

void MissionUI::Update()
{
	ratio_ += 0.005f;
	pText_->SetRatio(Easing::EaseInQuad(ratio_),0.5f);
}

void MissionUI::Draw()
{
	pText_->Draw();
}

void MissionUI::Release()
{
	SAFE_DELETE(pText_);
}
