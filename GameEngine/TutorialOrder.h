#pragma once
#include"MissionUI.h"

class TutorialOrder : public MissionUI
{
private:
	Text* pText_;
	float ratio_;
	float alpha_;
	int hPict_;

public:
	TutorialOrder(GameObject* parent);
	~TutorialOrder();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void EraseText();
	void Release() override;
};

