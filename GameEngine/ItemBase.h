#pragma once
#include"Engine/GameObject/GameObject.h"
#include"Engine/ResourceManager/Model.h"
#include"Engine/DirectX_11/Particle.h"
enum class ITEM_TYPE
{
	ITEM_NONE = 0,
	SPEED,
	SEARCH,
	ITEM_MAX,
};

class ItemBase : public GameObject
{
private:
	ITEM_TYPE itemType_;
	bool isCollision_;
	bool isAttached_;
	int iniTime_;
	int lifeTime_;
	GameObject* attacheObject_;
	float defaultParameter_;
	void SetIniTime(int time) { iniTime_ = time; }
public:
	Particle* pParticle_;
	ItemBase(GameObject* parent, std::string itemName);
	virtual ~ItemBase() {};
	void AttachItem(GameObject* target = nullptr);
	ITEM_TYPE GetItemType();
	int GetLifeTime();
	float GetDefaultParameter() { return defaultParameter_; }
	void SetItemType(ITEM_TYPE type);
	void SetLifeTime(int time);
	void SetDefaultParameter(float param);
	GameObject* GetAttacheObject() { return attacheObject_; }
	bool IsAttach();
	void SetCollision();
	bool IsCollision();
	void Reset() { lifeTime_ = iniTime_; }
	virtual void PlayParticle(XMFLOAT3 pos) {};
	void SubtractTime(int time=1);

};