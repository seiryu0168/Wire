#pragma once
#include<string>
enum class COMPONENT_TYPE
{
	COMP_MODEL = 0,
};
class Component
{
private:
	COMPONENT_TYPE componentType_;
public:
	Component();
	Component(COMPONENT_TYPE type);
	virtual ~Component() {};

	COMPONENT_TYPE GetType();
};

