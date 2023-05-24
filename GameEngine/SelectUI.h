#pragma once
#include"Engine/GameObject/GameObject.h"
#include"Engine/ResourceManager/json.hpp"

class Text;
class SelectUI : public GameObject
{
	using json = nlohmann::json;
private:
	enum class UI_MODE
	{
		MODE_INPUT = 0,
		MODE_MOVE,
		MODE_SELECTED,
	};
	struct button_
	{
		Text* buttonText_;
		int hButtonPict_;
		int hMissionPict_;
		XMFLOAT3 position_;
	};
	short buttonMove_;
	json* fileReader_;
	UI_MODE uiMode_;
	int hPictBackGround_;
	int hPictButtonFrame_;
	int buttonCount_;
	int buttonNum_;
	int moveTime_;
	int inputInterval_;
	void Input();
	void Move();
	std::vector<button_> buttonList_;
public:
	SelectUI(GameObject* parent);
	~SelectUI();
	void Initialize() override;
	void Update() override;
	void ThirdDraw() override;
	void PushedButton(int num);
	void MoveButton(float ratio);
	bool IsLimit(int buttonNum);
	void ReadFile(std::string fileName);
	void LoadImageFile();
	void Release() override;
};