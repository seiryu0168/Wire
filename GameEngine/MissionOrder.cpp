#include "MissionOrder.h"
#include"Engine/ResourceManager/ImageManager.h"
#include"Easing.h"
#include"ObjectSetter.h"
MissionOrder::MissionOrder(ObjectSetter* p)
	:ratio_(0),
	pText_(nullptr),
	pCountText_(nullptr),
	hPict_(-1)

{
	TEXT_RECT rect1 = { 0,0,500,100 };
	TEXT_RECT rect2 = { 0,0,700,200 };
	hPict_ = ImageManager::Load("Assets\\TextBackGround.png");
	assert(hPict_ >= 0);
	pText_ = new Text();
	pText_->Load("敵を倒せ！", "Sitka Text", rect1, LEFT_TOP);
	pSetter_ = p;
	EnemyMax_=pSetter_->GetEnemyCount();
	EnemyCount_=pSetter_->GetEnemyCount();
	pCountText_ = new Text();
	count_ = std::to_string(EnemyCount_);
	pCountText_->Load("Enemy:" + count_, "Sitka Text", rect2, LEFT_TOP);

}

MissionOrder::~MissionOrder()
{
}

void MissionOrder::Update()
{

	ratio_ += 0.015f;
	//ratio_が1以上になったら
	if (ratio_ >= 1.0f)
	{
		//アルファ値減らす
		pText_->SetColor({ 1,1,1,max(0,1.0f - (ratio_ - 1.0f)) });
		ImageManager::SetAlpha(hPict_, max(0, 1.0f - (ratio_ - 1.0f)));
	}
	EnemyCount_ = pSetter_->GetEnemyCount();
	count_ = std::to_string(EnemyCount_);
	pCountText_->SetText("Enemy:"+count_);
	//テキスト移動
	pText_->SetRatio(Easing::EaseOutQuint(ratio_) * 0.4f, 0.45f);
}

void MissionOrder::Draw()
{
	pCountText_->Draw();
	pText_->Draw();
}

void MissionOrder::Release()
{
	SAFE_RELEASE(pText_);
	SAFE_RELEASE(pCountText_);

}
