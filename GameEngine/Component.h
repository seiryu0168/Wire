#pragma once
#include"Engine/GameObject/GameObject.h"
enum class COMPONENT_TYPE
{
	COMP_MODEL = 0,
};
class Component
{
private:
	COMPONENT_TYPE componentType_;
	GameObject* pObject_;
	bool active_;
public:
	Component();
	Component(GameObject* obj);
	virtual ~Component() {};
	virtual void Update(bool active) = 0;
	void SetObject(GameObject* obj);
	void SetActive(bool active);
	bool IsActive() { return active_; }
	virtual void Release() = 0;
	GameObject* GetAttachObject();
	COMPONENT_TYPE GetType();
};

