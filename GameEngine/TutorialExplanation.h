#pragma once]
#include<string>
#include"Engine/ResourceManager/json.hpp"
class Text;
class TutorialExplanation
{
	using json = nlohmann::json;
	json tutorialFile_;
	std::vector<Text*> textList_;
	std::vector<int> imageHandleList_;
public:
	TutorialExplanation();
	~TutorialExplanation();
	void Initialize();
	void Update();
	bool Load(std::string fileName);
	void Draw();
};

