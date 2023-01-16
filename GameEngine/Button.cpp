#include "Button.h"
Button::Button(std::string fileName)
	:fileName_(""),
	isSelect_(0),
	hPictButton_(-1)

{
	hPictButton_ = ImageManager::Load(fileName);
	assert(hPictButton_);
}

Button::~Button()
{
}