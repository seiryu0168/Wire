#pragma once
#include"ItemBase.h"
class SearchUpItem : public ItemBase
{
private:
	int hModel_;
	float value_;
public:
	SearchUpItem(GameObject* parent);
	~SearchUpItem();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	float GetItemParam() { return value_; }
	void Release() override;
	void OnCollision(GameObject* pTarget) override;
};

