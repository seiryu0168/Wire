#pragma once
#include"SceneBase.h"
class TutorialScene : public SceneBase
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

