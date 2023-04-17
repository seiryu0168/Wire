#include "Component.h"

Component::Component()
{
}

Component::Component(COMPONENT_TYPE type)
{
	componentType_ = type;
}

COMPONENT_TYPE Component::GetType()
{
	return componentType_;
}
