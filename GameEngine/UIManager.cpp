#include "UIManager.h"
#include"Engine/ResourceManager/ImageManager.h"
#include "MissionOrder.h"
#include"TutorialOrder.h"
#include<d3d11.h>
#include<fstream>
#include<sstream>
namespace
{
	static const std::string UI_DATA = "UIData";
	static const std::string FILE_NAME[2] = { "TutorialStageUIData.json",
											  "Stage1UIData.json" };
}

UIManager::UIManager()
	:uiData_(nullptr),
	pObject_(nullptr)
{
}

UIManager::~UIManager()
{
}

void UIManager::Initialize(int stageNum)
{
	//�X�e�[�W�ԍ����t�@�C���̍ő吔�ȏゾ������t�@�C���̍ő吔�ɒ���
	int maxStageCount = sizeof(FILE_NAME) / sizeof(std::string);
	if (maxStageCount <= stageNum)
		stageNum = maxStageCount - 1;
	LoadFile(FILE_NAME[stageNum]);
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
				bool isRectData = txtData[1].is_array();
				assert(isRectData);
				auto rectData = txtData[1];
				auto text = txtData[0].get<std::string>();
				
				//json�t�@�C������Rect�̔z�������Ă���rect�^�ϐ��ɓ����
				TEXT_RECT rect = { rectData.at(0).get<int>(),
								   rectData.at(1).get<int>(),
								   rectData.at(2).get<int>(),
								   rectData.at(3).get<int>() };
				pText->Load(text, "Sitka Text", rect, LEFT_TOP);
				mission.textList_.push_back(pText);
			}
		}
		if (j.is_object())
		{
			for (auto& imgData : j)
			{
				auto imageName = imgData[0].get<std::string>();
				auto imagePosArray = imgData[1];
				XMFLOAT3 imagePos = { imagePosArray.at(0).get<float>(),
									  imagePosArray.at(1).get<float>(),
									  imagePosArray.at(2).get<float>() };
				int imageHandle = ImageManager::Load("Assets\\Image\\" + imageName);
				ImageManager::SetImagePos(imageHandle,imagePos);
				mission.imageList_.push_back(imageHandle);
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
	//�J�����g�f�B���N�g���擾
	WCHAR currentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, currentDir);

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
		//�擾�������O�����Ƃ�UI�I�u�W�F�N�g����
		std::string uiName = elem.value().items().begin().key();
		MissionUI* pMission;
		pMission=CreateUI(uiName);
		//�e�L�X�g���X�g�̐ݒ�
		if (uiList_.size() > i)
		{
			pMission->SetText(uiList_[i].textList_);
			pMission->SetImage(uiList_[i].imageList_);
			i++;
		}
	}
}
