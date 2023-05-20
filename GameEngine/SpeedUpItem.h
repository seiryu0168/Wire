#pragma once
#include"ItemBase.h"

class SpeedUpItem : public ItemBase
{
private:
	int hModel_;
	float value_;
public:
	SpeedUpItem(GameObject* parent);
	~SpeedUpItem();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	float GetItemParam() { return value_; }
	void Release() override;
	void OnCollision(GameObject* pTarget) override;
};

