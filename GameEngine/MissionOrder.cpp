#include "MissionOrder.h"
#include"Engine/ResourceManager/ImageManager.h"
#include"Easing.h"
#include"ObjectSetter.h"
namespace
{
	static const XMFLOAT2 TEXT_POS = { 0.4f,0.45f };
	static const std::string ORDER = "敵を倒せ！";
	static const std::string ENEMY = "Enemy:";
	static const float DELTA_RATIO = 0.015f;
	static const float DELTA_ALPHA = 0.01f;
	static const std::string BACKGROUND_IMAGE = "Assets\\Image\\TextBackGround.png";
}
MissionOrder::MissionOrder(GameObject* parent)
	:MissionUI(parent,"MissionOrder"),
	ratio_(0),
	pText_(nullptr),
	pCountText_(nullptr),
	hPict_(-1),
	alpha_(1.0f),
	EnemyCount_(0),
	EnemyMax_(0),
	pSetter_(nullptr)

{
	hPict_ = ImageManager::Load(BACKGROUND_IMAGE);
	assert(hPict_ >= 0);

}

MissionOrder::~MissionOrder()
{
	Release();
}

void MissionOrder::Initialize()
{
	TEXT_RECT rect1 = { 0,0,500,100 };
	TEXT_RECT rect2 = { 0,0,700,200 };
	pText_ = new Text();
	pText_->Load(ORDER, "Sitka Text", rect1, LEFT_TOP);
	pCountText_ = new Text();
	pCountText_->Load(ENEMY + count_, "Sitka Text", rect2, LEFT_TOP,90);
	pSetter_ = (ObjectSetter*)FindObject("ObjectSetter");
	EnemyMax_=pSetter_->GetEnemyCount();
	EnemyCount_=pSetter_->GetEnemyCount();
	count_ = std::to_string(EnemyCount_);
}

void MissionOrder::Update()
{
	ratio_ += DELTA_RATIO;
	//ratio_が1以上になったら
	if (ratio_ >= 1.0f)
	{
		//テキストと背景を徐々に薄くしていく
		EraseText();
	}
	EnemyCount_ = pSetter_->GetEnemyCount();
	count_ = std::to_string(EnemyCount_);
	textList[1]->SetText(ENEMY + count_);
	//テキスト移動
	textList[0]->SetRatio(Easing::EaseOutQuint(ratio_) * TEXT_POS.x, TEXT_POS.y);
}

void MissionOrder::Draw()
{
}

void MissionOrder::ThirdDraw()
{
	ImageManager::Draw(hPict_);
	for (auto& i : textList)
	{
		i->Draw();
	}

	//通知テキスト
	for (auto& i : noticeTextList_)
	{

		i.second->Draw();
	}
}

void MissionOrder::EraseText()
{
	//アルファ値減らしていく
	textList[0]->SetTextColor({ 1,1,1,alpha_ });
	ImageManager::SetAlpha(hPict_, alpha_);
	alpha_ -= DELTA_ALPHA;
}

void MissionOrder::Release()
{
	SAFE_RELEASE(pText_);
	SAFE_RELEASE(pCountText_);

}
