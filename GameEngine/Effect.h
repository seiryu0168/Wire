#pragma once
#include <string>
#include "Engine/Transform.h"

namespace Effect
{
	int Load(std::string fileName);
	void Draw(int effNum);
	void SetTransform(int effNum, Transform transform);
	void Release();
	
};

