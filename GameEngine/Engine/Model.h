#pragma once
#include<string>
#include"Fbx.h"
#include"Transform.h"
#include<vector>
namespace ModelManager
{
	int Load(std::string fileName);
	void SetTransform(int modelNum, Transform transform);
	void Draw(int modelName);
	void RayCast(int modelNum, RayCastData& ray);
	void RayCast(RayCastData& ray);
	void SetModelNum(int modelNum);
	void DeleteModelNum(int modelNum);
	void Release();
};

