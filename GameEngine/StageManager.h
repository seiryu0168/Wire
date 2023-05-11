#pragma once
#include"Engine/ResourceManager/json.hpp"
class StageManager
{
	using json = nlohmann::json;
private:
	std::vector<std::string> stageName_;
public:
	StageManager();
	~StageManager();

	void Initialize(std::string fileName);
	int LoadStage(int stageNum);

};

