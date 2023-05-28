#pragma once
#include"Engine/GameObject/GameObject.h"
#include"Engine/ResourceManager/json.hpp"

class Text;
class PauseUI : public GameObject
{
	using json = nlohmann::json;
private:
	enum class UI_MODE
	{
		MODE_INVALID = 0,
		MODE_INPUT,
		MODE_MOVE,
		MODE_SELECTED,
	};
	struct button_
	{
		Text* buttonText_;
		XMFLOAT3 position_;
	};
	bool isPause_;
	XMFLOAT3 framePos_;
	short buttonMove_;
	json* fileReader_;
	UI_MODE uiMode_;
	int hPictBackGround_;
	int hPictButtonFrame_;
	int buttonCount_;
	int buttonNum_;
	int moveTime_;
	int inputInterval_;
	//ゲームの更新を止める
	void GameStop();
	//ゲームの更新を再開する
	void GameResume();
	//Aボタンが押されたらゲームの更新を止める
	void Invalid();
	//UIモードがINPUTならこの関数が呼ばれる
	void Input();
	//UIモードがMOVEならこの関数が呼ばれる
	void Move();
	std::vector<button_> buttonList_;
public:
	PauseUI(GameObject* parent);
	~PauseUI();
	void Initialize() override;
	void Update() override;
	void ThirdDraw() override;
	/// <summary>
	/// ボタン押した時の反応
	/// </summary>
	void PushedButton(int num);
	/// <summary>
	/// ボタン移動
	/// </summary>
	void MoveButton(float ratio);
	/// <summary>
	/// ファイル読み込み
	/// </summary>
	void ReadFile(std::string fileName);
	/// <summary>
	/// 読み込んだファイルから画像とテキスト情報を取り出す
	/// </summary>
	void LoadImageFile();
	void Release() override;
};