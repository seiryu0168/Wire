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
	//�t�@�C�����𒴂���ԍ������͂��ꂽ�璲��
	int maxFileCount = sizeof(FILE_NAME) / sizeof(std::string);
	if (maxFileCount < stageNum)
		stageNum = maxFileCount - 1;
	//�t�@�C����ǂݍ���
	bool success = LoadFile(FILE_NAME[stageNum]);
	assert(success);
}

void ItemManager::Update()
{
}

bool ItemManager::LoadFile(std::string fileName)
{
	//�J�����g�f�B���N�g���擾
	WCHAR currentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, currentDir);

	WCHAR path[FILENAME_MAX];

	//�f�B���N�g�����ړ�(�ł��Ȃ������玸�s)
	if (SetCurrentDirectory(L"Assets") == ERROR_FILE_NOT_FOUND)
	{
		return false;
	}
	//�t�@�C����ǂݎ��A�f�[�^��z��ɓ���Ă���
	std::ifstream file;
	file.open(fileName.c_str());
	if (file.good())
	{
		fileReader_ = json::parse(file);

		//json�I�u�W�F�N�g�̓L�[���w�肵�Ĕz���l�����
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
	//�t�@�C������A�f�B���N�g�������ɖ߂�
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
