#pragma once
#include"Engine/ResourceManager/json.hpp"
using json = nlohmann::json;
class StageManager
{
private:
	std::vector<std::string> stageName_;
public:
	StageManager();
	~StageManager();

	void Initialize(std::string fileName);
	int LoadStage(int stageNum);

};

