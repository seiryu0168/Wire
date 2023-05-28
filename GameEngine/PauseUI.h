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
	//�Q�[���̍X�V���~�߂�
	void GameStop();
	//�Q�[���̍X�V���ĊJ����
	void GameResume();
	//A�{�^���������ꂽ��Q�[���̍X�V���~�߂�
	void Invalid();
	//UI���[�h��INPUT�Ȃ炱�̊֐����Ă΂��
	void Input();
	//UI���[�h��MOVE�Ȃ炱�̊֐����Ă΂��
	void Move();
	std::vector<button_> buttonList_;
public:
	PauseUI(GameObject* parent);
	~PauseUI();
	void Initialize() override;
	void Update() override;
	void ThirdDraw() override;
	/// <summary>
	/// �{�^�����������̔���
	/// </summary>
	void PushedButton(int num);
	/// <summary>
	/// �{�^���ړ�
	/// </summary>
	void MoveButton(float ratio);
	/// <summary>
	/// �t�@�C���ǂݍ���
	/// </summary>
	void ReadFile(std::string fileName);
	/// <summary>
	/// �ǂݍ��񂾃t�@�C������摜�ƃe�L�X�g�������o��
	/// </summary>
	void LoadImageFile();
	void Release() override;
};