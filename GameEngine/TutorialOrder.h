#pragma once
#include"MissionUI.h"

class TutorialOrder : public MissionUI
{
private:
	Text* pText_;
	float ratio_;
	int hPict_;
public:
	TutorialOrder();
	~TutorialOrder();
	void Update() override;
	void Draw() override;
	void Release();
};

