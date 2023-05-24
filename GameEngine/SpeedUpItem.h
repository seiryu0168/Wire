#pragma once
#include"ItemBase.h"
class SpeedUpItem : public ItemBase
{
private:
	int hModel_;
	float value_;
	int hEmitter_;
	std::unique_ptr<EmitterData> pData_;
public:
	SpeedUpItem(GameObject* parent);
	~SpeedUpItem();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	float GetItemParam() { return value_; }
	void AttachedUpdate();
	void UnAttachedUpdate();
	void PlayParticle(XMFLOAT3 pos) override;
	void Release() override;
	void OnCollision(GameObject* pTarget) override;
};

