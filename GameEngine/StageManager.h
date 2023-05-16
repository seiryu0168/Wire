#pragma once
#include"Engine/ResourceManager/json.hpp"
class StageManager
{
	using json = nlohmann::json;
private:
	int stageCount_;
	std::vector<std::string> stageName_;
public:
	StageManager();
	~StageManager();

	void Initialize(std::string fileName);
	int MaxStage() { return stageCount_; }
	int LoadStage(int stageNum);

};

