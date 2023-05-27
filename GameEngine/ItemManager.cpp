#include "ItemManager.h"
#include"SpeedUpItem.h"
#include"SearchUpItem.h"
#include<d3d11.h>
#include<fstream>
#include<sstream>
namespace
{
	static const std::string FILE_NAME[4]{ "TutorialStageItemData.json",
										   "Stage1ItemData.json",
										   "Stage2ItemData.json",
										   "Stage3ItemData.json", };
}
ItemManager::ItemManager()
{
}

ItemManager::~ItemManager()
{
}

void ItemManager::Initialize(int stageNum)
{
	//ファイル数を超える番号が入力されたら調整
	int maxFileCount = sizeof(FILE_NAME) / sizeof(std::string);
	if (maxFileCount < stageNum)
		stageNum = maxFileCount - 1;
	//ファイルを読み込む
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

	//ディレクトリを移動(できなかったら失敗)
	if (SetCurrentDirectory(L"Assets") == ERROR_FILE_NOT_FOUND)
	{
		return false;
	}
	//ファイルを読み取り、データを配列に入れていく
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
	//ファイルを閉じ、ディレクトリを元に戻す
	file.close();
	SetCurrentDirectory(currentDir);
	return true;
}

ItemBase* ItemManager::CreateItem(std::string itemName)
{
	if (itemName == "SpeedUpItem")
	{
		return pObject_->Instantiate<SpeedUpItem>(pObject_);
	}
	if (itemName == "SearchUpItem")
	{
		return pObject_->Instantiate<SearchUpItem>(pObject_);
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
