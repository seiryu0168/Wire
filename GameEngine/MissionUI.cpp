#include "MissionUI.h"

MissionUI::MissionUI(GameObject* parent)
	:GameObject(parent,"MissionUI"),
	pText_(nullptr)
{
}

MissionUI::~MissionUI()
{
}

void MissionUI::Initialize()
{
	pText_ = new Text();
	TEXT_RECT rect = { 0,0,200,500 };
	pText_->Load("a", "Meiryo UI", rect, LEFT_TOP);
}

void MissionUI::Update()
{
}

void MissionUI::Draw()
{
	pText_->Draw();
}

void MissionUI::Release()
{
	SAFE_DELETE(pText_);
}
