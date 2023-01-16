#pragma once
#include"Button.h"
class PlayButton : public Button
{
private:
public:
	PlayButton(std::string fileName);
	~PlayButton();

	void Update() override;
	void Draw() override;
};