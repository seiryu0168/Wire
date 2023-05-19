#pragma once
#include"Engine/GameObject/GameObject.h"
#include"Engine/ResourceManager/Model.h"
enum class ITEM_TYPE
{
	ITEM_NONE = 0,
	UP_SPEED,
	UP_SEARCH,
	DOWN_SPEED,
	DOWN_SEARCH,
	ITEM_MAX,
};

class ItemBase : public GameObject
{
private:
	ITEM_TYPE itemType_;
	int lifeTime_;
	bool isAttached_;
public:
	ItemBase(GameObject* parent, std::string itemName);
	virtual ~ItemBase() {};
	void SetItemType(ITEM_TYPE type);
	void SetLifeTime(int time);
	ITEM_TYPE GetItemType();
	int GetLifeTime();

};

