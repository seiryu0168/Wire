#include "Component.h"

Component::Component()
	:componentType_(COMPONENT_TYPE::COMP_MODEL),
	pObject_(nullptr),
	active_(true)
{
}

Component::Component( GameObject* obj)
	:pObject_(obj),
	active_(true),
	componentType_(COMPONENT_TYPE::COMP_MODEL)
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
