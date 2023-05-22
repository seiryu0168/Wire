#include "SelectUI.h"
#include "SelectUI.h"
#include"Engine/ResourceManager/ImageManager.h"
#include"InterSceneData.h"
#include"Engine/DirectX_11/Input.h"
#include"Engine/SceneManager.h"
#include"Engine/ResourceManager/Text.h"
#include<fstream>
#include<sstream>
#include"Easing.h"

namespace
{
	static const std::string UI_IMAGE_FILE = "StageSelectUIImage.json";
	static const std::string BUTTON_LIST_NAME = "StageSelectUIImageList";
	static const int INTERVAL = 1;
	static const short UP = -1;
	static const short DOWN = 1;
	static const int MAX_BUTTON = 5;
	static const int DELAY = 90;
	static const int MOVE = 500;
	static const int MAX_MOVE_TIME = 11;
	static const XMFLOAT3 BUTTON_FRAME_POS = { -1400.0f,0.0f,0.0f };
}


SelectUI::SelectUI(GameObject* parent)
	:GameObject(parent, "SelectUI"),
	inputInterval_(INTERVAL),
	moveTime_(0),
	hPictBackGround_(-1),
	buttonMove_(0),
	canPushButton_(true),
	isMoveEnd_(true),
	buttonCount_(0)

{
	ReadFile(UI_IMAGE_FILE);
}

SelectUI::~SelectUI()
{
	for (auto& i : buttonList_)
	{
		SAFE_RELEASE(i.buttonText_);
	}
	SAFE_DELETE(fileReader_);
}

void SelectUI::Initialize()
{
	LoadImageFile();
}

void SelectUI::Update()
{
	switch (uiMode_)
	{
	case UI_MODE::MODE_INPUT:
		Input();
		break;
	case UI_MODE::MODE_MOVE:
		Move();
		break;
	}
}

void SelectUI::MoveButton(float ratio)
{
	float deltaPos = (MOVE / (MAX_MOVE_TIME - 1)) * buttonMove_;
	float delta = MOVE * ratio * buttonMove_;
	for (auto& i : buttonList_)
	{
		//�{�^�����ړ�������
		//i.position_.y += deltaPos;
		ImageManager::SetImagePos(i.hButtonPict_,
			XMFLOAT3(i.position_.x,
				i.position_.y + delta,
				0));
		ImageManager::SetImagePos(i.hMissionPict_,
			XMFLOAT3(400.0f,
				i.position_.y + delta,
				0));
		i.buttonText_->SetPosition({ i.position_.x - i.buttonText_->GetRect().right,
							   i.position_.y + i.buttonText_->GetRect().bottom + delta });
	}
}

bool SelectUI::IsLimit(int buttonNum)
{
	if (buttonNum > 0 || buttonNum < (buttonCount_ - 1))
		return true;
	else
		return false;
}

void SelectUI::Input()
{
	//�{�^���̏���
	buttonMove_ = 0;
	//��Ɉړ�
	if (Input::GetLStick_Y() >= 0.7f)
	{
		buttonMove_ = UP;
		//�{�^����
		if (buttonNum_ > 0)
			uiMode_ = UI_MODE::MODE_MOVE;
	}
	//���Ɉړ�
	else if (Input::GetLStick_Y() <= -0.7f)
	{
		buttonMove_ = DOWN;
		//�{�^���̔ԍ����Ō������Ȃ�������ړ����[�h�ɐ؂�ւ���
		if (buttonNum_ < (buttonCount_ - 1))
			uiMode_ = UI_MODE::MODE_MOVE;
	}
	//�{�^���̔ԍ��𒲐�
	buttonNum_ += buttonMove_;
	buttonNum_ = Clamp(buttonNum_, 0, buttonCount_ - 1);

	PushedButton(buttonNum_);
}

void SelectUI::Move()
{
	moveTime_++;
	moveTime_ = min(moveTime_, MAX_MOVE_TIME);
	//�����I�������
	if (moveTime_ >= MAX_MOVE_TIME)
	{
		uiMode_ = UI_MODE::MODE_INPUT;
		moveTime_ = 0;
		//���������Z���N�g��ʂ̈ʒu���m�肳����
		for (auto& i : buttonList_)
		{
			i.position_.y += MOVE * buttonMove_;
		}
		return;
	}
	//�Z���N�g��ʂ𓮂���
	MoveButton(Easing::EaseInCubic((float)moveTime_ / (float)(MAX_MOVE_TIME - 1)));
}


void SelectUI::ReadFile(std::string fileName)
{
	//�J�����g�f�B���N�g���擾
	WCHAR currentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, currentDir);

	if (SetCurrentDirectory(L"Assets") == ERROR_FILE_NOT_FOUND)
	{
		return;
	}
	std::ifstream fRead;
	fRead.open(fileName.c_str());
	if (fRead.good())
	{
		fileReader_ = new json;
		*fileReader_ = json::parse(fRead);
	}
	fRead.close();
	SetCurrentDirectory(currentDir);
}

void SelectUI::ThirdDraw()
{
	ImageManager::Draw(hPictBackGround_);
	ImageManager::Draw(hPictButtonFrame_);

	for (auto& i : buttonList_)
	{
		i.buttonText_->Draw();
	}
}

void SelectUI::PushedButton(int num)
{
	if (Input::IsPadButtonDown(XINPUT_GAMEPAD_A))
	{
		InterSceneData::SetData("StageNum", nullptr, &num, nullptr, nullptr);
		((SceneManager*)FindObject("SceneManager"))->ChangeScene(SCENE_ID::SCENE_ID_PLAY, DELAY);
	}
}

void SelectUI::LoadImageFile()
{
	//�^�C�g���摜�ǂݍ���
	hPictBackGround_ = ImageManager::Load("Assets\\Black.png");
	assert(hPictBackGround_ >= 0);
	//ImageManager::SetAlpha(hPictBackGround_, 0);
	//�{�^���摜�ǂݍ���
	TEXT_RECT rect = { 0,0,500,100 };
	for (auto elem : fileReader_[0][BUTTON_LIST_NAME].items().begin().value())
	{

		//�摜�̖��O���擾
		std::string buttonImageName = elem[0];
		std::string missionName = elem[1];

		/////////////////////////////�{�^���̗p��//////////////////////
		button_ btn;

		//�e�L�X�g�ǂݍ���
		btn.buttonText_ = new Text;
		btn.buttonText_->Load("Stage" + std::to_string(buttonCount_), "Sitka Text", rect, LEFT_TOP);

		//�摜�ǂݍ���
		btn.hButtonPict_ = ImageManager::Load("Assets\\" + buttonImageName);
		assert(btn.hButtonPict_ >= 0);
		btn.hMissionPict_ = ImageManager::Load("Assets\\" + missionName);
		assert(btn.hMissionPict_ >= 0);
		//�ʒu�ݒ�
		btn.position_ = { -1400.0f,(buttonCount_++) * (float)(-MOVE),0.0f };
		ImageManager::SetImagePos(btn.hButtonPict_, btn.position_);
		ImageManager::SetImagePos(btn.hMissionPict_, { 400.0f,buttonCount_ * (float)(-MOVE),0.0f });
		btn.buttonText_->SetPosition({ btn.position_.x - btn.buttonText_->GetRect().right,
								 btn.position_.y + btn.buttonText_->GetRect().bottom });
		//�z��ɓ����
		buttonList_.push_back(btn);
	}
	hPictButtonFrame_ = ImageManager::Load("Assets\\ButtonFrame.png");
	assert(hPictButtonFrame_ >= 0);

	ImageManager::SetImagePos(hPictButtonFrame_, BUTTON_FRAME_POS);
}

void SelectUI::Release()
{
}
