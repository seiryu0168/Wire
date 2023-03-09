#pragma once
#include"Engine/GameObject/GameObject.h"

class TitleUI : public GameObject
{
private:
	int hPictPlay_;
	int hPictPlay2_;
	int hPictTutorial_;
	int hPictTutorial2_;
	int hPictTitle_;
	int hPictButtonFrame_;
	int buttonNum_;
	int inputInterval_;

public:
	TitleUI(GameObject* parent);
	~TitleUI();
	void Initialize() override;
	void Update() override;
	void Release() override;
};