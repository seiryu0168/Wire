#pragma once
#include"Engine/Texture.h"
#include<string>
#include<vector>
namespace TextureManager
{
	int Load(std::string fileName);
	void Release();
}