#pragma once
#include"Engine/GameObject/GameObject.h"
#include"Text.h"
class MissionUI : public GameObject
{
	Text* pText_;
	float ratio_;
public:
	MissionUI(GameObject* parent);
	~MissionUI();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

