#include "ItemGetter.h"
#include"SpeedUpItem.h"
#include"SearchUpItem.h"
#include"Player.h"
#include"MissionUI.h"
namespace
{
	static const XMFLOAT2 NOTICE_POS = { -1800.0f,-400.0f };
	static const float NOTICE_INTERVAL = 400;
}
ItemGetter::ItemGetter(GameObject* object)
	:attachObject_(object)
{
	order_ = (MissionUI*)attachObject_->FindObjectAtTag("UI");
}

ItemGetter::~ItemGetter()
{
	SAFE_RELEASE(order_);
	itemList_.clear();
}

void ItemGetter::Update()
{
	//アイテムの効果が切れていないかどうか確認
	for (auto itr=itemList_.begin();itr!=itemList_.end();)
	{
		if ((*itr)->GetLifeTime() <= 0)
		{
			RemoveItemEffect((*itr));
			(*itr)->KillMe();
			//アイテム情報の通知を消す
			order_->DelNotice((*itr)->GetItemName()->c_str());
			itr = itemList_.erase(itr);
		}
		else
		{
			UpdateText(*itr);
			itr++;
		}
	}
}

void ItemGetter::ItemAttach(ItemBase* item)
{
	//同じアイテムを既に取得していたら
	ItemBase* itm=CheckSameItem(item->GetItemType());
	if (itm!=nullptr)
	{
		itm->Reset();
		return;
	}
	//アイテムの効果を有効にする
	item->AttachItem(attachObject_);
	Apply(item);
	//アイテム情報を生成、UIに通知
	CreateItemText(item);
	
	//アイテムリストに追加
	itemList_.push_back(item);
}

void ItemGetter::ItemRemove(ItemBase* item)
{
	//効果を消す
	RemoveItemEffect(item);
}

void ItemGetter::Apply(ItemBase* item)
{
	//アイテムのタイプによってプレイヤーのステータスを変える
	switch (item->GetItemType())
	{
	case ITEM_TYPE::SPEED:
		item->SetDefaultParameter(((Player*)attachObject_)->GetSpeed());
		((Player*)attachObject_)->SetSpeed(((SpeedUpItem*)item)->GetItemParam());
		break;
	case ITEM_TYPE::SEARCH:
		item->SetDefaultParameter(((Player*)attachObject_)->GetLockOnAngleLimit());
		((Player*)attachObject_)->SetLockOnAngleLimit(((SearchUpItem*)item)->GetItemParam());
		break;
	default:
		break;
	}

}

ItemBase* ItemGetter::CheckSameItem(ITEM_TYPE type)
{
	//同じアイテムがないかどうか調べる
	for (auto i : itemList_)
	{
		if (i->GetItemType() == type)
			return i;
	}
	return nullptr;
}

void ItemGetter::CreateItemText(ItemBase* item)
{
	//テキスト用意
	Text* text=new Text;
	
	//アイテム名と残り時間取得
	std::string effectName = item->GetItemName()->c_str();
	std::string effectTime= std::to_string(item->GetLifeTime() / 60);
	std::string txt = "ITEM:"+effectName + " " + effectTime + ":sec";
	
	//テキスト用矩形
	TEXT_RECT rect = { 0,0,500,50 };
	text->Load(txt, "Sitka Text", rect, LEFT_TOP);
	text->SetPosition(NOTICE_POS);
	HRESULT hr = text->SetTextSize(40);
	text->SetTextColor({ 1,1,1,1});
	text->SetBackColor({ 0,0,0,0.3f });
	order_->AddNotice(effectName, text);
}


void ItemGetter::UpdateText(ItemBase* item)
{
	//テキスト情報更新
	std::string effectName = item->GetItemName()->c_str();
	std::string effectTime = std::to_string(item->GetLifeTime() / 60);
	order_->ChangeNotice(item->GetItemName()->c_str(), "ITEM:" + effectName + " " + effectTime + ":sec");
}

void ItemGetter::RemoveItemEffect(ItemBase* item)
{
	//アイテムタイプによってプレイヤーに与える効果を変える
	switch (item->GetItemType())
	{
	case ITEM_TYPE::SPEED:
		((Player*)attachObject_)->SetSpeed(item->GetDefaultParameter());
		break;
	case ITEM_TYPE::SEARCH:
		((Player*)attachObject_)->SetLockOnAngleLimit(item->GetDefaultParameter());
		break;
	default:
		break;
	}
}
