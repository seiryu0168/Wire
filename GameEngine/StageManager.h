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
		StageData()
		{
			areaLimit_ = { 0,0,0,0 };
			stageName_ = "";
		}
	};
	std::vector<StageData*> stageDatas_;
public:
	StageManager();
	~StageManager();
	//ステージ名読み込み
	void Initialize(std::string fileName);
	//ステージの最大数取得
	int MaxStage() { return stageCount_; }
	//ステージの範囲取得
	const XMFLOAT4& GetAreaLimit(int stageNum);
	//ステージのモデル読み込み
	int LoadStage(int stageNum);

};

