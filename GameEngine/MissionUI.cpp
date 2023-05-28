#include "MissionUI.h"
#include"Engine/ResourceManager/ImageManager.h"
#include"Easing.h"
namespace
{
	static const XMFLOAT2 FIRST_NOTICE_POS = { -1800.0f,-700.0f };
}
MissionUI::MissionUI(GameObject* parent, std::string uiName)
	:GameObject(parent,uiName)
{
	SetTag("UI");
}

MissionUI::~MissionUI()
{
	textList.clear();
	noticeTextList_.clear();
}

void MissionUI::SetText(std::vector<Text*> txtList)
{
	textList = txtList;
}

void MissionUI::SetImage(std::vector<int> imgList)
{
	imageList = imgList;
}

void MissionUI::AddNotice(std::string key, Text* text)
{
	//通知追加
	noticeTextList_[key] = text;
	CalcTextDistance();
}

void MissionUI::ChangeNotice(std::string key, std::string text)
{
	//通知内容を更新する
	if (noticeTextList_.find(key) != noticeTextList_.end())
		noticeTextList_[key]->SetText(text);
}

void MissionUI::DelNotice(std::string key)
{
	//通知を消す
	auto itr = noticeTextList_.find(key);
	if (itr != noticeTextList_.end())
	{
		SAFE_RELEASE(itr->second);
		noticeTextList_.erase(itr);
	}

	CalcTextDistance();
}

void MissionUI::CalcTextDistance()
{
	int totalMove = 0;
	int move = 0;
	
	//基準の位置からの距離を計算して順に並べる
	for (auto itr = noticeTextList_.begin(); itr != noticeTextList_.end(); itr++)
	{
		totalMove += move;
		(*itr).second->SetPosition({ FIRST_NOTICE_POS.x,
									 FIRST_NOTICE_POS.y+totalMove });
		move = (*itr).second->GetRect().bottom;
	}
}

void MissionUI::Release()
{
}
