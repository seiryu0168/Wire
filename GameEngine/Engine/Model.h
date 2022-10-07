#pragma once
#include<string>
#include"Fbx.h"
#include"Transform.h"
namespace Model
{
	int Load(std::string fileName);
	void SetTransform(int modelNum, Transform transform);
	void Draw(int modelName);
	void RayCast(int modelNum, RayCastData& ray);
	void Release();
};

