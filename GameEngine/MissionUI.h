#pragma once
#include "Engine/GameObject/GameObject.h"
#include"Engine/ResourceManager/Text.h"
class MissionUI : public GameObject
{
public:
	std::vector<Text*> textList;
	std::vector<int> imageList;

	MissionUI(GameObject* parent,std::string uiName);
	~MissionUI();

	void SetText(std::vector<Text*> txtList);
	void SetImage(std::vector<int> imgList);
	void Release();
};

