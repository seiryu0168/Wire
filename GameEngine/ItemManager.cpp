#include "ItemManager.h"
#include"SpeedUpItem.h"
#include<d3d11.h>
#include<fstream>
#include<sstream>
namespace
{
	static const std::string FILE_NAME[4]{ "Stage1ItemData.json",
										   "Stage2ItemData.json",
										   "Stage3ItemData.json",
										   "Stage4ItemData.json", };
}
ItemManager::ItemManager()
{
}

ItemManager::~ItemManager()
{
}

void ItemManager::Initialize(int stageNum)
{
	int maxStageCount = sizeof(FILE_NAME) / sizeof(std::string);
	if (maxStageCount < stageNum)
		stageNum = maxStageCount-1;
	bool success = LoadFile(FILE_NAME[stageNum]);
	assert(success);
}

void ItemManager::Update()
{
}

bool ItemManager::LoadFile(std::string fileName)
{
	//カレントディレクトリ取得
	WCHAR currentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, currentDir);

	WCHAR path[FILENAME_MAX];

	if (SetCurrentDirectory(L"Assets") == ERROR_FILE_NOT_FOUND)
	{
		return false;
	}
	std::ifstream file;
	file.open(fileName.c_str());
	if (file.good())
	{
		fileReader_ = json::parse(file);

		//jsonオブジェクトはキーを指定して配列や値を取る
		for (auto& elem : fileReader_["ItemData"].items())
		{
			ItemData* data = new ItemData;
			itemCount_++;

			data->itemName_ = elem.value()[0];
			data->position_ = { elem.value()[1][0],
								elem.value()[1][1],
								elem.value()[1][2] };
			itemDatas_.push_back(data);
		}
	}
	else
		return false;
	file.close();
	SetCurrentDirectory(currentDir);
}

ItemBase* ItemManager::CreateItem(std::string itemName)
{
	if (itemName == "SpeedUpItem")
	{
		return pObject_->Instantiate<SpeedUpItem>(pObject_);
	}
	return nullptr;
}

void ItemManager::SetItem()
{
	for (auto& i : itemDatas_)
	{
		ItemBase* pItem;
		pItem=CreateItem(i->itemName_);
		pItem->SetPosition(i->position_);
	}
}

void ItemManager::SetParentObject(GameObject* parent)
{
	pObject_ = parent;
}
