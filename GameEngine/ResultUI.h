#pragma once
#include"Engine/GameObject/GameObject.h"
#include"Engine/ResourceManager/json.hpp"

class Text;
	enum class UI_MODE
	{
		MODE_INPUT = 0,
		MODE_MOVE,
	};
class ResultUI : public GameObject
{
	using json = nlohmann::json;
private:
	int hPictResult_;
	int hPictRetry_;
	int hPictButtonFrame_;
	short buttonMove_;

	bool isMoveEnd_;
	bool canPushButton_;
	json* fileReader_;
	UI_MODE uiMode_;
	int hPictTitle_;
	int buttonCount_;
	int buttonNum_;
	int prevNum_;
	int moveTime_;
	int inputInterval_;
	struct button_
	{
		Text* buttonText_;
		Text* informationText_;
		XMFLOAT3 position_;
	};
	void Input();
	void Move();
	std::vector<button_> buttonList_;
public:
	ResultUI(GameObject* parent);
	~ResultUI();

	void Initialize() override;

	void Update() override;

	void PushedButton(int num);
	void MoveButton(float ratio);
	bool IsLimit(int buttonNum);
	void ReadFile(std::string fileName);
	void LoadImageFile();
	void ThirdDraw() override;
	void Release() override;
}; 