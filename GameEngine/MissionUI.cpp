#include "MissionUI.h"
#include"Engine/ResourceManager/ImageManager.h"
#include"Easing.h"

MissionUI::MissionUI(GameObject* parent, std::string uiName)
	:GameObject(parent,uiName)
{
}

MissionUI::~MissionUI()
{
}

void MissionUI::SetText(std::vector<Text*> txtList)
{
	textList = txtList;
}

void MissionUI::SetImage(std::vector<int> imgList)
{
	imageList = imgList;
}

void MissionUI::Release()
{
}
