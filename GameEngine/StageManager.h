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
	//�X�e�[�W���ǂݍ���
	void Initialize(std::string fileName);
	//�X�e�[�W�̍ő吔�擾
	int MaxStage() { return stageCount_; }
	//�X�e�[�W�͈͎̔擾
	const XMFLOAT4& GetAreaLimit(int stageNum);
	//�X�e�[�W�̃��f���ǂݍ���
	int LoadStage(int stageNum);

};

