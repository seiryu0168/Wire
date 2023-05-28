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
	GameObject* pObject_;
	int itemCount_;
	json fileReader_;
public:
	ItemManager();
	~ItemManager();
	//番号を調整してファイルを読み込む
	void Initialize(int stageNum);
	//ファイル読み込み
	bool LoadFile(std::string fileName);
	//アイテム生成
	ItemBase* CreateItem(std::string itemName);
	//アイテム設置
	void SetItem();
	//アイテムを置く親オブジェクトを設定
	void SetParentObject(GameObject* parent);
};

