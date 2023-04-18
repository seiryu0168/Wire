#include "Component.h"

Component::Component()
	:componentType_(COMPONENT_TYPE::COMP_MODEL),
	pObject_(nullptr)
{
}

Component::Component(COMPONENT_TYPE type, GameObject* obj)
	:componentType_(type),
	pObject_(obj)
{
}

void Component::SetObject(GameObject* obj)
{
	pObject_ = obj;
}

GameObject* Component::GetAttachObject()
{
	return pObject_;
}

COMPONENT_TYPE Component::GetType()
{
	return componentType_;
}
