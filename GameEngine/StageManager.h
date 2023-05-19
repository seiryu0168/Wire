#pragma once
#include"Engine/ResourceManager/json.hpp"
#include"Engine/DirectX_11/Math.h"
class StageManager
{
	using json = nlohmann::json;
private:
	int stageCount_;
	struct StageData
	{
		XMFLOAT4 areaLimit_;
		std::string stageName_;
	};
	std::vector<StageData*> stageDatas_;
public:
	StageManager();
	~StageManager();

	void Initialize(std::string fileName);
	int MaxStage() { return stageCount_; }
	const XMFLOAT4& GetAreaLimit(int stageNum);
	int LoadStage(int stageNum);

};

