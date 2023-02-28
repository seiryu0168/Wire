#pragma once
#include"Engine/GameObject/GameObject.h"
class PlayerBase : public GameObject
{
private:
	int hModel_;
public:
	PlayerBase(GameObject* parent);
	~PlayerBase();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

