#pragma once
#include<string>
#include"Engine/ResourceManager/json.hpp"
#include"Engine/DirectX_11/Math.h"
class Text;
class TutorialExplanation
{
	struct imageData
	{
		int hPict_;
		XMFLOAT3 position_;
	};
	enum class INPUT_MODE
	{
		MODE_INPUT = 0,
		MODE_MOVE,
		MODE_SELECTED,
	};

	int moveTime_;
	int slideNum_;
	int hFramePict_;
	int hTutorialBackGroundPict_;
	short moveDir_;
	INPUT_MODE mode_;
	using json = nlohmann::json;
	json tutorialFile_;
	std::vector<Text*> textList_;
	std::vector<imageData> imageList_;
	void Input();
	void Move();
public:
	TutorialExplanation();
	~TutorialExplanation();
	void Initialize();
	void Update();
	void MoveSlide(float ratio);
	bool Load(std::string fileName);
	void Draw();
};

