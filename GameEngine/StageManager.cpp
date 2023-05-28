#include "StageManager.h"
#include<d3d11.h>
#include<fstream>
#include<sstream>
#include"Engine/ResourceManager/Model.h"
//#include"Engine/GameObject/GameObject.h"

StageManager::StageManager()
	:stageCount_(0)
{
}

StageManager::~StageManager()
{
}

void StageManager::Initialize(std::string fileName)
{
	//�J�����g�f�B���N�g���擾
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
		json stageNameArray;
		fileReader >> stageNameArray;
		//json�I�u�W�F�N�g�̓L�[���w�肵�Ĕz���l�����
		for (auto& elem : stageNameArray["StageName"].items())
		{
			StageData* data = new StageData;
			stageCount_++;
			
			data->areaLimit_ = { elem.value()[1][0],
								 elem.value()[1][1],
								 elem.value()[1][2],
								 elem.value()[1][3] };
			data->stageName_=elem.value()[0];
			stageDatas_.push_back(data);
		}
	}
	fileReader.close();
	SetCurrentDirectory(currentDir);
}

const XMFLOAT4& StageManager::GetAreaLimit(int stageNum)
{
	//�X�e�[�W�͈̔͂��擾
	if (stageDatas_.size() < stageNum)
		return XMFLOAT4(1, 1, 1, 1);
	return stageDatas_[stageNum]->areaLimit_;
}

int StageManager::LoadStage(int stageNum)
{
	//�X�e�[�W�̃��f���ǂݍ���
	std::string filePath = "Assets\\Model\\" + stageDatas_[stageNum]->stageName_ + ".fbx";
	int hModel = ModelManager::Load(filePath);
	assert(hModel >= 0);
	return hModel;
}
