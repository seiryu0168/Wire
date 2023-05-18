#pragma once
#include"Engine/ResourceManager/json.hpp"
#include"MissionUI.h"
class UIManager
{
	using json = nlohmann::json;
private:
	struct MissionParam
	{
		std::vector<Text*> textList_;
		std::vector<int> imageList_;
	};
	json* uiData_;
	std::vector<MissionParam> uiList_;
	std::vector<std::string> uiName_;
	GameObject* pObject_;
public:
	UIManager();
	~UIManager();

	void Initialize(int stageNum);
	void SetParentObject(GameObject* parent);
	void LoadFile(std::string fileName);
	MissionUI* CreateUI(std::string uiName);
	void SetUI();
};

