#pragma once
#include"Engine/GameObject/GameObject.h"
#include"ItemBase.h"
#include"Engine/ResourceManager/Text.h"
class MissionUI;
class ItemGetter
{
private:
	std::list<ItemBase*> itemList_;
	GameObject* attachObject_;
	MissionUI* order_;
	std::vector<Text> textList_;
public:
	ItemGetter(GameObject* object);
	~ItemGetter();
	void Update();
	void ItemAttach(ItemBase* item);
	void ItemRemove(ItemBase* item);
	void Apply(ItemBase* item);
	ItemBase* CheckSameItem(ITEM_TYPE type);
	void CreateItemText(ItemBase* item);
	void UpdateText(ItemBase*item);	
	void RemoveItemEffect(ItemBase* item);
};

