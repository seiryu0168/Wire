#include "StageManager.h"
#include<d3d11.h>
#include<typeinfo>
#include<fstream>
#include<sstream>
#include"Engine/ResourceManager/Model.h"
//#include"Engine/GameObject/GameObject.h"

StageManager::StageManager()
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
			stageName_.push_back(elem.value());
		}
	}
	fileReader.close();
	SetCurrentDirectory(currentDir);
}

int StageManager::LoadStage(int stageNum)
{
	std::string filePath = "Assets\\" + stageName_[stageNum] + ".fbx";
	int hModel = ModelManager::Load(filePath);
	assert(hModel >= 0);
	return hModel;
}
