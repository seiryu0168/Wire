#pragma once
#include"Engine/GameObject.h"
class ResultUI : public GameObject
{
private:
	int hPict_;
	int hPictTitle_;
	int hPictRetry_;
public:
	ResultUI(GameObject* parent);
	~ResultUI();

	void Initialize() override;

	void Update() override;

	void Release() override;
};