#pragma once
#include"Engine/GameObject/GameObject.h"

class TitleUI : public GameObject
{
private:
	int hPictTitle_;
	int hPictMessage_;
	int flashingTime_;
	bool isButtonPush_;
public:
	TitleUI(GameObject* parent);
	~TitleUI();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};