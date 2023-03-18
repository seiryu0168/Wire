#include "MissionUI.h"
#include"Engine/ResourceManager/ImageManager.h"
#include"Easing.h"

MissionUI::MissionUI()
	:pText_(nullptr)
{
	pText_ = new Text();
}

MissionUI::~MissionUI()
{
}

//void MissionUI::Update()
//{
//	
//}

//void MissionUI::Draw()
//{
//}

void MissionUI::Release()
{
	SAFE_DELETE(pText_);
}
