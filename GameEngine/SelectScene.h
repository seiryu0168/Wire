#pragma once
#include"SceneBase.h"
class SelectScene : SceneBase
{
private:

public:
	SelectScene(GameObject* parent);
	~SelectScene();
	void Initialize() override;
	void Release() override;
};

