#include "UIManager.h"
#include "MissionOrder.h"
#include"TutorialOrder.h"
#include<d3d11.h>
#include<fstream>
#include<sstream>
namespace
{
	static const std::string UI_DATA = "UIData";
}
UIManager::UIManager()
{
}

UIManager::~UIManager()
{
}

void UIManager::Initialize(std::string fileName)
{
	LoadFile(fileName);
	for (auto& elem : uiData_[0][UI_DATA].items())
	{
		elem.value().items().begin().key();
		uiList_
	}
}

void UIManager::SetParentObject(GameObject* parent)
{
	pObject_ = parent;
}

void UIManager::LoadFile(std::string fileName)
{
	//カレントディレクトリ取得
	WCHAR currentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, currentDir);

	WCHAR path[FILENAME_MAX];

	if (SetCurrentDirectory(L"Assets") == ERROR_FILE_NOT_FOUND)
	{
		return;
	}
	std::ifstream fileReader;
	fileReader.open(fileName.c_str());
	if (fileReader.good())
	{
		uiData_ =new json;
		*uiData_ = json::parse(fileReader);
	}
	fileReader.close();
	SetCurrentDirectory(currentDir);
}

MissionUI* UIManager::CreateUI(std::string uiName)
{
	MissionUI* p = nullptr;
	if (uiName == "MissionOrder")
	{
		p=pObject_->Instantiate<MissionOrder>(pObject_);
	}
	if (uiName == "TutorialOrder")
	{
		p = pObject_->Instantiate<TutorialOrder>(pObject_);
	}
	if (p == nullptr)
		return nullptr;
	
	return p;
}

void UIManager::SetUI()
{
	for (auto& elem : uiData_[0]["uiDataList"].items())
	{
		std::string uiName = elem.value().items().begin().key();
		//CreateUI()
	}
}
