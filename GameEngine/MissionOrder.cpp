#include "MissionOrder.h"
#include"Engine/ResourceManager/ImageManager.h"
#include"Easing.h"
#include"ObjectSetter.h"
MissionOrder::MissionOrder(ObjectSetter* p)
	:ratio_(0),
	hPict_(-1)

{
	TEXT_RECT rect = { 0,0,500,100 };
	hPict_ = ImageManager::Load("Assets\\TextBackGround.png");
	assert(hPict_ >= 0);

	GetTextPointer()->Load("�G��|���I", "Meiryo UI", rect, LEFT_TOP);
	pSetter_ = p;
	EnemyMax_=pSetter_->GetEnemyCount();
	EnemyCount_=pSetter_->GetEnemyCount();
	pCountText_ = new Text();
	count_ = std::to_string(EnemyCount_);
	pCountText_->Load("�c��" + count_, "Mairyo UI", { 0,0,1000,200 }, LEFT_TOP);

}

MissionOrder::~MissionOrder()
{
}

void MissionOrder::Update()
{

	ratio_ += 0.015f;
	//ratio_��1�ȏ�ɂȂ�����
	if (ratio_ >= 1.0f)
	{
		//�A���t�@�l���炷
		GetTextPointer()->SetColor({ 1,1,1,max(0,1.0f - (ratio_ - 1.0f)) });
		ImageManager::SetAlpha(hPict_, max(0, 1.0f - (ratio_ - 1.0f)));
	}
	EnemyCount_ = pSetter_->GetEnemyCount();
	count_ = std::to_string(EnemyCount_);
	pCountText_->SetText("�c��"+count_);
	//�e�L�X�g�ړ�
	GetTextPointer()->SetRatio(Easing::EaseOutQuint(ratio_) * 0.4f, 0.45f);
}

void MissionOrder::Draw()
{
	pCountText_->Draw();
	GetTextPointer()->Draw();
}

void MissionOrder::Release()
{
}
