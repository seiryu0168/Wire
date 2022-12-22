#include "TextureManager.h"

namespace TextureManager
{
	struct textureData
	{
		Texture* pTexture_;
		std::string fileName_;
	};
	std::vector<Texture*> textureList_;
}
int TextureManager::Load(std::string fileName)
{
	/*textureData* ptextureData = new textureData;
	ptextureData->pTexture_ = nullptr;
	
	for (auto itr = textureList_.begin(); itr != textureList_.end(); ire++)
	{
		if(fileName==(*itr)->)
	}
	if(fileName)*/
}

void TextureManager::Release()
{

}