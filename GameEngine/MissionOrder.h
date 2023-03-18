#pragma once
#include"MissionUI.h"
class ObjectSetter;
class MissionOrder : public MissionUI
{
private:
	Text* pText_;
	Text* pCountText_;
	std::string count_;
	int hPict_;
	float ratio_;
	int EnemyMax_;
	int EnemyCount_;
	ObjectSetter* pSetter_;

public:
	MissionOrder( ObjectSetter* p);
	~MissionOrder();
	void Update();
	void Draw();
	void Release();
};

