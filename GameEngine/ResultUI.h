#pragma once
#include"Engine/GameObject/GameObject.h"
class ResultUI : public GameObject
{
private:
	int hPict_;
	int hPictTitle_;
	int hPictRetry_;

	int inputInterval_;
	int buttonNum_;
public:
	ResultUI(GameObject* parent);
	~ResultUI();

	void Initialize() override;

	void Update() override;

	void Release() override;
};