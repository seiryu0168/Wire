#pragma once
#include"Engine/ImageManager.h"
#include"Engine/GameObject.h"
class Button
{
private:
	std::string fileName_;
	bool isSelect_;
	int hPictButton_;
	RECT rect_;

public:
	Button(std::string fileName);
	~Button();
	HRESULT Load(std::string fileName);
	virtual void Update() {};
	bool Select() { return isSelect_; }
	virtual void Draw() {};

};

