#pragma once
#include<string>
#include"../DirectX_11/Fbx.h"
#include"../GameObject/Transform.h"
#include<vector>
namespace ModelManager
{
	int Load(std::string fileName);
	void SetTransform(int modelNum, Transform transform);
	void Draw(int modelNum);
	void DrawOutLine(int modelNum);
	void DrawToon(int modelNum);
	void RayCast(int modelNum, RayCastData& ray);
	void RayCast(RayCastData& ray);
	void SetModelNum(int modelNum);
	void SetShader(std::string shaderName);
	void DeleteModelNum(int modelNum);
	void Release();
};

