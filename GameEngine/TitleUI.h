#pragma once
#include"Engine/GameObject.h"

class TitleUI : public GameObject
{
private:
	int hPictPlay_;
	int hPictQuit_;
	int hPictTitle_;

public:
	TitleUI(GameObject* parent);
	~TitleUI();
	void Initialize() override;
	void Update() override;
	void Release() override;
};