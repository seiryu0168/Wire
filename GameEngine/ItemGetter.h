#pragma once
#include"Engine/GameObject/GameObject.h"
#include"ItemBase.h"
class ItemGetter
{
private:
	std::list<ItemBase*> itemList_;
	GameObject* attachObject_;
public:
	ItemGetter(GameObject* object);
	~ItemGetter();
	void Update();
	void ItemAttach(ItemBase* item);
	void ItemRemove(ItemBase* item);
	void Apply(ItemBase* item);
	bool CheckSameItem(ITEM_TYPE type);
	void RemoveItemEffect(ItemBase* item);
};

