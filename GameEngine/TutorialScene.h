#pragma once
#include"Engine/GameObject/GameObject.h"
class TutorialScene : public GameObject
{
private:

public:
	TutorialScene(GameObject* parent);
	~TutorialScene();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

