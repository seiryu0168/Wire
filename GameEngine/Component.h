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
public:
	Component();
	Component(COMPONENT_TYPE type, GameObject* obj);
	virtual ~Component() {};
	virtual void Update(bool active) = 0;
	void SetObject(GameObject* obj);
	GameObject* GetAttachObject();
	COMPONENT_TYPE GetType();
};

