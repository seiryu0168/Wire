#include "TutorialOrder.h"
#include"Engine/ResourceManager/ImageManager.h"
#include"Easing.h"
namespace 
{
	static const XMFLOAT2 TEXT_POS = { 0.4f,0.45f };
	static const float DELTA_RATIO = 0.015f;
	static const float DELTA_ALPHA = 0.01f;
	static const std::string BACKGROUND_IMAGE = "Assets\\Image\\TextBackGround.png";
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
	pText_->Load("�`���[�g���A��", "Sitka Text", rect,LEFT_TOP);
	hPict_ = ImageManager::Load(BACKGROUND_IMAGE);
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
	//ratio_��1�ȏ�ɂȂ�����
	if (ratio_ >= 1.0f)
	{
		//�e�L�X�g�Ɣw�i�����X�ɔ�������
		EraseText();
	}

	//�e�L�X�g�ړ�
	textList[0]->SetRatio(Easing::EaseInOutQuint(ratio_) * TEXT_POS.x, TEXT_POS.y);
}

void TutorialOrder::Draw()
{
	//�`��
	textList[0]->Draw();
	ImageManager::Draw(imageList[0]);
	for (auto& i : noticeTextList_)
	{
		i.second->Draw();
	}
}

void TutorialOrder::EraseText()
{
	//�A���t�@�l���炵�Ă���
	textList[0]->SetColor({ 1,1,1,alpha_ });
	ImageManager::SetAlpha(hPict_,alpha_);
	alpha_ -= DELTA_ALPHA;
}

void TutorialOrder::Release()
{
	SAFE_RELEASE(pText_);
}
