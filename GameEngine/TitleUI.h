#pragma once
#include"Engine/GameObject/GameObject.h"
#include"Engine/ResourceManager/json.hpp"
using json = nlohmann::json;
class TitleUI : public GameObject
{
private:
	enum class UI_MODE
	{
		MODE_INPUT = 0,
		MODE_MOVE,
	};
	struct button_
	{
		int hPict_;
		XMFLOAT3 position_;
	};
	short buttonMove_;
	json fileReader_;
	bool isMoveEnd_;
	bool canPushButton_;
	UI_MODE uiMode_;
	int hPictTitle_;
	int hPictButtonFrame_;
	int buttonNum_;
	int prevNum_;
	int moveTime_;
	int inputInterval_;
	void Input();
	void Move();
	std::vector<button_> buttonList_;
public:
	TitleUI(GameObject* parent);
	~TitleUI();
	void Initialize() override;
	void Update() override;
	void MoveButton(int num);
	void MoveToEnd(int buttonNum);
	int IsEndorStart(int buttonNum);
	void ReadFile(std::string fileName);
	void Release() override;
};