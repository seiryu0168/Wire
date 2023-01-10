#include "ImageManager.h"
#include"Engine/Sprite.h"

namespace ImageManager
{
	struct imageData
	{
		std::string fileName_;
		Sprite* pSprite_;
		RECT rect_;
		float alpha_;
		Transform transform_;
		imageData()
		{
			fileName_ = "";
			pSprite_ = nullptr;
			alpha_ = 1.0f;
		}
	};
	std::vector<imageData*> imageList_;
}

int ImageManager::Load(std::string fileName)
{
	imageData* pImage = new imageData;
	pImage->fileName_ = fileName;
	for (int i = 0; i < imageList_.size(); i++)
	{
		if (imageList_[i]->fileName_ == fileName)
		{
			pImage->pSprite_ = imageList_[i]->pSprite_;
			break;
		}
	}

	if (pImage->pSprite_ == nullptr)
	{
		pImage->pSprite_ = new Sprite;
		if (FAILED(pImage->pSprite_->Load(fileName)))
		{
			SAFE_DELETE(pImage->pSprite_);
			SAFE_DELETE(pImage);
			return -1;
		}
	}

	return imageList_.size() - 1;
}

void ImageManager::Draw(int imageHandle)
{
	imageList_[imageHandle]->pSprite_->Draw(imageList_[imageHandle]->transform_);
}

void ImageManager::ResetRect(int imageHandle)
{
	if (imageHandle < 0 || imageHandle > imageList_.size())
	{
		return;
	}
	//XMFLOAT3 size = ;
}
