#pragma once
#include"SceneBase.h"
class StageSelectScene : public SceneBase
{
public:
	StageSelectScene(GameObject* parent);
	~StageSelectScene();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

