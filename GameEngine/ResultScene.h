#pragma once
#include"SceneBase.h"
class ResultScene : public SceneBase
{
public:
	ResultScene(GameObject* parent);
	~ResultScene();

	void Initialize() override;

	void Update() override;

	void Release() override;
};

