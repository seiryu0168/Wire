#include "ItemGetter.h"
#include"SpeedUpItem.h"
#include"SearchUpItem.h"
#include"Player.h"
#include"MissionOrder.h"
namespace
{
	static const XMFLOAT2 NOTICE_POS = { -1800.0f,-400.0f };
	static const float NOTICE_INTERVAL = 400;
}
ItemGetter::ItemGetter(GameObject* object)
	:attachObject_(object)
{
	mOrder_ = (MissionOrder*)attachObject_->FindObject("MissionOrder");
}

ItemGetter::~ItemGetter()
{
	itemList_.clear();
}

void ItemGetter::Update()
{
	for (auto itr=itemList_.begin();itr!=itemList_.end();)
	{
		if ((*itr)->GetLifeTime() <= 0)
		{
			RemoveItemEffect((*itr));
			(*itr)->KillMe();
			mOrder_->DelNotice((*itr)->GetObjectName());
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
	//�����A�C�e�������Ɏ擾���Ă�����
	if (CheckSameItem(item->GetItemType()))
	{
		return;
	}
	//�A�C�e���̌��ʂ�L���ɂ���
	item->AttachItem(attachObject_);

	Apply(item);
	CreateItemText(item);
	
	//�A�C�e�����X�g�ɒǉ�
	itemList_.push_back(item);
}

void ItemGetter::ItemRemove(ItemBase* item)
{
	//���ʂ�����
	RemoveItemEffect(item);
}

void ItemGetter::Apply(ItemBase* item)
{
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

bool ItemGetter::CheckSameItem(ITEM_TYPE type)
{
	for (auto i : itemList_)
	{
		if (i->GetItemType() == type)
			return true;
	}
	return false;
}

void ItemGetter::CreateItemText(ItemBase* item)
{
	//�e�L�X�g�p��
	Text* text=new Text;
	
	//�A�C�e�����Ǝc�莞�Ԏ擾
	std::string effectName = item->GetObjectName();
	std::string effectTime= std::to_string(item->GetLifeTime() / 60);
	std::string txt = effectName + ":" + effectTime + "sec";
	
	//�e�L�X�g�p��`
	TEXT_RECT rect = { 0,0,500,250 };
	text->Load(txt, "Sitka Text", rect, LEFT_TOP);
	text->SetPosition(NOTICE_POS);
	int j = text->GetTextSize();
	HRESULT hr=text->SetTextSize(10, 0, txt.size());
	int q = text->GetTextSize();


	mOrder_->AddNotice(effectName, text);
}


void ItemGetter::UpdateText(ItemBase* item)
{
	std::string effectName = item->GetObjectName();
	std::string effectTime = std::to_string(item->GetLifeTime() / 60);
	mOrder_->ChangeNotice(item->GetObjectName(), effectName + ":" + effectTime + "sec");
}

void ItemGetter::RemoveItemEffect(ItemBase* item)
{
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
