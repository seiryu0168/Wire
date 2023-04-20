#pragma once

#include"Engine/ResourceManager/Text.h"
class MissionUI
{
public:
	MissionUI();
	~MissionUI();
	
	virtual void Update() {};
	virtual void Draw() {};
	void Release();
};

