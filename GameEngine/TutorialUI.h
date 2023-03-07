#pragma once
#include"Engine/GameObject/GameObject.h"
class TutorialUI : public GameObject
{
private:

public:
	TutorialUI(GameObject* parent);
	~TutorialUI();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

