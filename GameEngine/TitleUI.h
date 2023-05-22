#pragma once
#include"Engine/GameObject/GameObject.h"

class TitleUI : public GameObject
{
private:
	int hPictTitle_;
	int hPictMessage_;
public:
	TitleUI(GameObject* parent);
	~TitleUI();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};