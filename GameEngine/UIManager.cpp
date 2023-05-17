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
		MissionParam mission;
		std::string uiName = elem.value().begin().key();
		auto i = elem.value()[uiName].items().begin().value()["TextList"];
		auto j = elem.value()[uiName].items().begin().value()["ImageList"];
		if (i.is_object())
		{

			for (auto& txtData : i)
			{
				Text* pText = new Text();
				TEXT_RECT rect = { 0,0,500,500 };
				auto k = txtData.begin().value().get<std::string>();
				pText->Load(k, "Sitka Text", rect, LEFT_TOP);
				mission.textList_.push_back(pText);

			}
		}
		if (j.is_object())
		{

			for (auto& imgData : j)
			{
				auto k = imgData[0];
				//mission.textList_.push_back()
					//uiList_
			}
		}
		uiList_.push_back(mission);
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
	if (uiName == "MissionOrder")
	{
		return pObject_->Instantiate<MissionOrder>(pObject_);
	}
	if (uiName == "TutorialOrder")
	{
		return pObject_->Instantiate<TutorialOrder>(pObject_);
	}
		return nullptr;
}

void UIManager::SetUI()
{
	int i = 0;
	for (auto& elem : uiData_[0]["UIData"].items())
	{
		std::string uiName = elem.value().items().begin().key();
		MissionUI* pMission;
		pMission=CreateUI(uiName);
		pMission->SetText(uiList_[i].textList_);
		i++;
	}
}
