#include "TutorialOrder.h"
#include"Engine/ResourceManager/ImageManager.h"
#include"Easing.h"
TutorialOrder::TutorialOrder()
	:pText_(nullptr),
	ratio_(0),
	hPict_(-1)
{
	pText_ = new Text();
	TEXT_RECT rect = { 0,0,500,100 };
	pText_->Load("�`���[�g���A��", "Meiryo UI", rect,LEFT_TOP);
	hPict_ = ImageManager::Load("Assets\\TextBackGround.png");
}

TutorialOrder::~TutorialOrder()
{
}

void TutorialOrder::Update()
{
	ratio_ += 0.015f;
	//ratio_��1�ȏ�ɂȂ�����
	if (ratio_ >= 1.0f)
	{
		//�e�L�X�g�Ɣw�i�𓧖��ɂ���
		pText_->SetColor({ 1,1,1,max(0,1.0f - (ratio_ - 1.0f)) });
		ImageManager::SetAlpha(hPict_, max(0, 1.0f - (ratio_ - 1.0f)));
	}

	//�e�L�X�g�ړ�
	pText_->SetRatio(Easing::EaseInOutQuint(ratio_)*0.4f, 0.45f);
}

void TutorialOrder::Draw()
{
	pText_->Draw();
}

void TutorialOrder::Release()
{
	SAFE_RELEASE(pText_);
}
