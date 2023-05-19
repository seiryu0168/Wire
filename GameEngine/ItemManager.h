#pragma once
#include"Engine/ResourceManager/json.hpp"
#include"ItemBase.h"
#include"Engine/DirectX_11/Math.h"
class ItemManager
{
private:
	using json = nlohmann::json;
	struct ItemData
	{
		std::string itemName_;
		XMFLOAT3 position_;
	};
	std::vector<ItemData*> itemDatas_;
	int itemCount_;
	json fileReader_;
public:
	ItemManager();
	~ItemManager();
	void Initialize(int stageNum);
	void Update();
	void LoadFile(std::string fileName);
	ItemBase* CreateItem(std::string itemName);
	void SetItem();
};

