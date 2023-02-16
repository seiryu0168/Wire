#pragma once
#include"Engine/GameObject/GameObject.h"
class ResultScene : public GameObject
{
public:
	ResultScene(GameObject* parent);
	~ResultScene();

	void Initialize() override;

	void Update() override;

	void Release() override;
};

