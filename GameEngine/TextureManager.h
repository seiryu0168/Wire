#pragma once
#include"Engine/Texture.h"
#include<string>
#include<vector>
namespace TextureManager
{
	int Load(std::string fileName);
	Texture* GetTexture(int textureHandle);
	void Release();
}