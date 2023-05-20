#pragma once
#include"Engine/GameObject/GameObject.h"
#include"Engine/ResourceManager/Model.h"
enum class ITEM_TYPE
{
	ITEM_NONE = 0,
	SPEED,
	SEARCH,
	ITEM_MAX,
};

class ItemBase : public GameObject
{
private:
	ITEM_TYPE itemType_;
	bool isAttached_;
	int lifeTime_;
	float defaultParameter_;
public:
	ItemBase(GameObject* parent, std::string itemName);
	virtual ~ItemBase() {};
	void AttachItem();
	ITEM_TYPE GetItemType();
	int GetLifeTime();
	float GetDefaultParameter() { return defaultParameter_; }
	void SetItemType(ITEM_TYPE type);
	void SetLifeTime(int time);
	void SetDefaultParameter(float param);
	bool IsAttach();
	void SubtractTime(int time=1);

};

