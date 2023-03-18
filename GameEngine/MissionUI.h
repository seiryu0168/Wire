#pragma once

#include"Text.h"
class MissionUI
{
	Text* pText_;
	int hPict_;
	float ratio_;
public:
	MissionUI();
	~MissionUI();
	
	virtual void Update() {};
	virtual void Draw() {};
	Text* GetTextPointer() { return pText_; }
	void Release();
};

