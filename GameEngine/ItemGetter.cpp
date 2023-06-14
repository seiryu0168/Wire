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
	//�A�C�e���̌��ʂ��؂�Ă��Ȃ����ǂ����m�F
	for (auto itr=itemList_.begin();itr!=itemList_.end();)
	{
		if ((*itr)->GetLifeTime() <= 0)
		{
			RemoveItemEffect((*itr));
			(*itr)->KillMe();
			//�A�C�e�����̒ʒm������
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
	//�����A�C�e�������Ɏ擾���Ă�����
	ItemBase* itm=CheckSameItem(item->GetItemType());
	if (itm!=nullptr)
	{
		itm->Reset();
		return;
	}
	//�A�C�e���̌��ʂ�L���ɂ���
	item->AttachItem(attachObject_);
	Apply(item);
	//�A�C�e�����𐶐��AUI�ɒʒm
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
	//�A�C�e���̃^�C�v�ɂ���ăv���C���[�̃X�e�[�^�X��ς���
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
	//�����A�C�e�����Ȃ����ǂ������ׂ�
	for (auto i : itemList_)
	{
		if (i->GetItemType() == type)
			return i;
	}
	return nullptr;
}

void ItemGetter::CreateItemText(ItemBase* item)
{
	//�e�L�X�g�p��
	Text* text=new Text;
	
	//�A�C�e�����Ǝc�莞�Ԏ擾
	std::string effectName = item->GetItemName()->c_str();
	std::string effectTime= std::to_string(item->GetLifeTime() / 60);
	std::string txt = "ITEM:"+effectName + " " + effectTime + ":sec";
	
	//�e�L�X�g�p��`
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
	//�e�L�X�g���X�V
	std::string effectName = item->GetItemName()->c_str();
	std::string effectTime = std::to_string(item->GetLifeTime() / 60);
	order_->ChangeNotice(item->GetItemName()->c_str(), "ITEM:" + effectName + " " + effectTime + ":sec");
}

void ItemGetter::RemoveItemEffect(ItemBase* item)
{
	//�A�C�e���^�C�v�ɂ���ăv���C���[�ɗ^������ʂ�ς���
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
