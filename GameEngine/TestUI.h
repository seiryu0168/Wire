#pragma once
#include"MissionUI.h"
class TestUI : public MissionUI
{
public:
	TestUI(GameObject* parent);
	~TestUI();
	void Initialize() override;
	void Draw() override;
	void Release() override;

};

