#pragma once
#include"MissionUI.h"
#include"TutorialExplanation.h"
class TutorialOrder : public MissionUI
{
private:
	Text* pText_;
	float ratio_;
	float alpha_;
	bool isTutorial_;
	int hPict_;
	int hPictBackButton_;
	TutorialExplanation te_	;

public:
	TutorialOrder(GameObject* parent);
	~TutorialOrder();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void ThirdDraw() override;
	void ChangeScreen();
	void EraseText();
	void Release() override;
};

