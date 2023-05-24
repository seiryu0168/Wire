#pragma once
#include"ItemBase.h"
class SearchUpItem : public ItemBase
{
private:
	int hModel_;
	float value_;
	int hEmitter_;
	std::unique_ptr<EmitterData> pData_;
public:
	SearchUpItem(GameObject* parent);
	~SearchUpItem();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	float GetItemParam() { return value_; }
	void Release() override;
	void PlayParticle(XMFLOAT3 pos) override;
	void OnCollision(GameObject* pTarget) override;
};

