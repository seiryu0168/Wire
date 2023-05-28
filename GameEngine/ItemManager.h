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
	//�ԍ��𒲐����ăt�@�C����ǂݍ���
	void Initialize(int stageNum);
	//�t�@�C���ǂݍ���
	bool LoadFile(std::string fileName);
	//�A�C�e������
	ItemBase* CreateItem(std::string itemName);
	//�A�C�e���ݒu
	void SetItem();
	//�A�C�e����u���e�I�u�W�F�N�g��ݒ�
	void SetParentObject(GameObject* parent);
};

