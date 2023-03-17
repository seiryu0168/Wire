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
	pText_->Load("�G��|���I", "Meiryo UI", rect, LEFT_TOP);
}

void MissionUI::Update()
{
	ratio_ += 0.01f;
	//ratio_��1�ȏ�ɂȂ�����
	if (ratio_ >= 1.0f)
	{
		//�A���t�@�l���炷
		pText_->SetColor({ 1,1,1,max(0,1 - (ratio_ - 1.0f)) });
	}
	//�e�L�X�g�ړ�
	pText_->SetRatio(Easing::EaseOutQuint(ratio_)*0.4f,0.5f);
	//ratio_��3�ȏ�ɂȂ����玀��
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
