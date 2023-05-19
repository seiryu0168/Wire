#pragma once
#include"Engine/GameObject/GameObject.h"
#include"ItemBase.h"
class ItemGetter
{
private:
	std::vector<ItemBase*> itemList_;
	GameObject* attachObject_;
public:
	ItemGetter(GameObject* object);
	~ItemGetter();
	void ItemAttach();
	void ItemRemove();

};

