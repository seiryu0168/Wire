#include "Component.h"

Component::Component()
	:componentType_(COMPONENT_TYPE::COMP_MODEL),
	pObject_(nullptr),
	active_(true)
{
}

Component::Component(COMPONENT_TYPE type, GameObject* obj)
	:componentType_(type),
	pObject_(obj),
	active_(true)
{
}

void Component::SetObject(GameObject* obj)
{
	pObject_ = obj;
}

void Component::SetActive(bool active)
{
	active_ = active;
}

GameObject* Component::GetAttachObject()
{
	return pObject_;
}

COMPONENT_TYPE Component::GetType()
{
	return componentType_;
}
