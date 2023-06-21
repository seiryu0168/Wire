#include "ResultUI.h"
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
	static const std::string UI_IMAGE_FILE = "ResultUIData.json";
	static const std::string BUTTON_LIST_NAME = "ResultUIDataList";
	static const float STICK_TILT = 0.7f;
	static const int INFORMATION_OFFSET = 1400;
	static const int INTERVAL = 1;

	static const int TITLE = 0;
	static const int SELECT = 1;
	static const int PLAY = 2;
	static const short UP = -1;
	static const short DOWN = 1;
	static const int MAX_BUTTON = 5;
	static const int DELAY = 90;
	static const int MOVE = 300;
	static const int MAX_MOVE_TIME = 11;
	static const XMFLOAT3 BUTTON_FIRST_POS = { 0.0f,-100.0f,0.0f };

	static const std::string CLEAR_IMAGE = "Assets\\Image\\Clear.png";
	static const std::string GAMEOVER_IMAGE = "Assets\\Image\\GameOver.png";
}


ResultUI::ResultUI(GameObject* parent)
	:GameObject(parent, "ResultUI"),
	inputInterval_(INTERVAL),
	moveTime_(0),
	hPictTitle_(-1),
	buttonMove_(0),
	canPushButton_(true),
	isMoveEnd_(true),
	buttonCount_(0),
	buttonFramePos_(BUTTON_FIRST_POS),
	uiMode_(UI_MODE::MODE_INPUT)

{
	ReadFile(UI_IMAGE_FILE);
}

ResultUI::~ResultUI()
{

	//�������
	for (auto& i : buttonList_)
	{
		SAFE_RELEASE(i.buttonText_);
	}
	SAFE_DELETE(fileReader_);
	buttonList_.clear();
}

void ResultUI::Initialize()
{
	//�N���A�������ǂ���
	bool isClear=InterSceneData::GetboolData("Result");
	if (isClear)
	{
		hPictResult_ = ImageManager::Load(CLEAR_IMAGE);
	}
	else
	{
		hPictResult_ = ImageManager::Load(GAMEOVER_IMAGE);
	}
	
	LoadImageFile();
}

void ResultUI::Update()
{
	//���̓��[�h���ړ����[�h���I�����[�h��
	switch (uiMode_)
	{
	case UI_MODE::MODE_INPUT:
		Input();
		break;
	case UI_MODE::MODE_MOVE:
		Move();
		break;
	case UI_MODE::MODE_SELECTED:
		break;
	}
}

void ResultUI::MoveButton(float ratio)
{
	//�ő�ړ��ʁ�����������
	int delta = -(MOVE * ratio * buttonMove_);
		//�{�^�����ړ�������
		ImageManager::SetImagePos(hPictButtonFrame_,
								  {buttonFramePos_.x,
								   buttonFramePos_.y+ delta,
								   buttonFramePos_.z });
}

bool ResultUI::IsLimit(int buttonNum)
{
	if (buttonNum > 0 || buttonNum < (buttonCount_ - 1))
		return true;
	else
		return false;
}

void ResultUI::Input()
{
	//�{�^���̏���
	buttonMove_ = 0;
	//��Ɉړ�
	if (Input::GetLStick_Y() >= STICK_TILT)
	{
		buttonMove_ = UP;
		//�{�^����
		if (buttonNum_ > 0)
			uiMode_ = UI_MODE::MODE_MOVE;
	}
	//���Ɉړ�
	else if (Input::GetLStick_Y() <= -STICK_TILT)
	{
		buttonMove_ = DOWN;
		//�{�^���̔ԍ����Ō������Ȃ�������ړ����[�h�ɐ؂�ւ���
		if (buttonNum_ < (buttonCount_ - 1))
			uiMode_ = UI_MODE::MODE_MOVE;
	}
	//�{�^���̔ԍ��𒲐�
	buttonNum_ += buttonMove_;
	buttonNum_ = Clamp<float>((float)buttonNum_, 0.0f, (float)(buttonCount_ - 1));

	PushedButton(buttonNum_);
}

void ResultUI::Move()
{
	moveTime_++;
	moveTime_ = min(moveTime_, MAX_MOVE_TIME);
	//�����I�������
	if (moveTime_ >= MAX_MOVE_TIME)
	{
		//UI���[�h�ؑւ�
		uiMode_ = UI_MODE::MODE_INPUT;
		moveTime_ = 0;
		//���������Z���N�g��ʂ̈ʒu���m�肳����
		buttonFramePos_.y += -(MOVE * buttonMove_);
		return;
	}
	//�Z���N�g��ʂ𓮂���
	MoveButton(Easing::EaseInCubic((float)moveTime_ / (float)(MAX_MOVE_TIME - 1)));
}


void ResultUI::ReadFile(std::string fileName)
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

void ResultUI::ThirdDraw()
{
	ImageManager::Draw(hPictResult_);
	for (auto& i : buttonList_)
	{
		i.buttonText_->Draw();
	}
}

void ResultUI::PushedButton(int num)
{
	if (Input::IsPadButtonDown(XINPUT_GAMEPAD_A))
	{
		switch (num)
		{
		case TITLE:
			((SceneManager*)FindObject("SceneManager"))->ChangeScene(SCENE_ID::SCENE_ID_TITLE, DELAY);
			break;
		case SELECT:
			((SceneManager*)FindObject("SceneManager"))->ChangeScene(SCENE_ID::SCENE_ID_SELECT, DELAY);
			break;

		case PLAY:
			((SceneManager*)FindObject("SceneManager"))->ChangeScene(SCENE_ID::SCENE_ID_PLAY, DELAY);
			break;
		}
		uiMode_ = UI_MODE::MODE_SELECTED;
	}
}

void ResultUI::LoadImageFile()
{
	hPictButtonFrame_ = ImageManager::Load("Assets\\Image\\ButtonFrame.png");
	assert(hPictButtonFrame_ >= 0);

	ImageManager::SetImagePos(hPictButtonFrame_, BUTTON_FIRST_POS);
	ImageManager::SetUIList(hPictButtonFrame_);
	
	//�{�^���摜�ǂݍ���
	for (auto elem : fileReader_[0][BUTTON_LIST_NAME].items().begin().value())
	{
		std::string type = elem[0];
		if (type == "Button")
		{
			buttonCount_++;
		}
		//�摜�̖��O���擾
		std::string button = elem[1];

		//�e�L�X�g�p��`
		TEXT_RECT rect = { 0 };
		rect = { elem[2][0],elem[2][1],elem[2][2],elem[2][3] };
		
		//�{�^�����
		button_ btn;
		//�e�L�X�g�ǂݍ���
		btn.buttonText_ = new Text;
		btn.buttonText_->Load(button, "Sitka Text", rect, LEFT_TOP);

		//�ʒu�ݒ�
		btn.position_ = { elem[3][0],
						  elem[3][1],
						  elem[3][2]};
		btn.buttonText_->SetPosition({ btn.position_.x - btn.buttonText_->GetRect().right,
									   btn.position_.y + btn.buttonText_->GetRect().bottom });
		buttonList_.push_back(btn);
	}
}

void ResultUI::Release()
{
}
