#include "TutorialExplanation.h"
#include"Engine/ResourceManager/ImageManager.h"
#include"Engine/ResourceManager/Text.h"
#include<d3d11.h>
#include<fstream>
#include<sstream>
namespace 
{
	static const std::string FILENAME = "TutorialExplanation.json";
}
TutorialExplanation::TutorialExplanation()
{
}

TutorialExplanation::~TutorialExplanation()
{
}

void TutorialExplanation::Initialize()
{
	if (Load(FILENAME) == false)
		return;


	for(auto elem : tutorialFile_)


}

void TutorialExplanation::Update()
{
}

bool TutorialExplanation::Load(std::string fileName)
{
	//カレントディレクトリ取得
	WCHAR currentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, currentDir);

	if (SetCurrentDirectory(L"Assets") == ERROR_FILE_NOT_FOUND)
	{
		return false;
	}
	std::ifstream fileReader;
	fileReader.open(fileName.c_str());
	if (fileReader.good() == false)
	{
		return false;
	}
	tutorialFile_ = json::parse(fileReader);
	fileReader.close();
	SetCurrentDirectory(currentDir);
	return true;
}

void TutorialExplanation::Draw()
{
}
