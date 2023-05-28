#include "PauseUI.h"
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
	static const std::string UI_DATA_FILE = "PauseUIData.json";
	static const std::string BUTTON_LIST_NAME = "PauseUIDataList";
	static const float STICK_TILT = 0.7f;
	static const int INFORMATION_OFFSET = 1400;
	static const int INTERVAL = 1;
	static const int TITLE = 0;
	static const int PLAY = 1;
	static const short LEFT = -1;
	static const short RIGHT = 1;
	static const int MAX_BUTTON = 5;
	static const int DELAY = 90;
	static const int MOVE = 1200;
	static const int MAX_MOVE_TIME = 11;
	static const XMFLOAT3 BUTTON_FIRST_POS = { -600.0f,-400.0f,0.0f };
	static const std::string BACKGROUND_IMAGE = "Assets\\Image\\Black.png";
	static const std::string BUTTONFLAME_IMAGE = "Assets\\Image\\ButtonFrame.png";
}


PauseUI::PauseUI(GameObject* parent)
	:GameObject(parent, "PauseUI"),
	uiMode_(UI_MODE::MODE_INVALID),
	inputInterval_(INTERVAL),
	moveTime_(0),
	hPictBackGround_(-1),
	hPictButtonFrame_(-1),
	buttonMove_(0),
	buttonCount_(0),
	framePos_(BUTTON_FIRST_POS),
	isPause_(false)
{
	ReadFile(UI_DATA_FILE);
}


PauseUI::~PauseUI()
{
	for (auto& i : buttonList_)
	{
		SAFE_RELEASE(i.buttonText_);	
	}
	SAFE_DELETE(fileReader_);
}

void PauseUI::Initialize()
{
	LoadImageFile();
}

void PauseUI::Update()
{

		switch (uiMode_)
		{
		case UI_MODE::MODE_INVALID:
			Invalid();
			break;
		case UI_MODE::MODE_INPUT:
			Input();
			break;
		case UI_MODE::MODE_MOVE:
			Move();
			break;
		case UI_MODE::MODE_SELECTED:
			Selected();
			break;
		}
}

void PauseUI::MoveButton(float ratio)
{
	int deltaPos = (MOVE / (MAX_MOVE_TIME - 1)) * buttonMove_;
	int delta = MOVE * ratio * buttonMove_;
	XMFLOAT3 pos = framePos_;
	pos.x += delta;
	ImageManager::SetImagePos(hPictButtonFrame_, pos);
}

void PauseUI::GameStop()
{

	GameObject* p = GetScene();
	//�V�[�����̓���I�u�W�F�N�g�������čX�V�𖳌��ɂ���
	for (auto i : *(GetScene()->GetChildList()))
	{
		if (i->GetObjectName() != this->GetObjectName()&&
			i->GetObjectName()!="ObjectSetter")
		{
			i->SetUpdate(false);
		}
	}
}

void PauseUI::GameResume()
{
	//�V�[�����̃I�u�W�F�N�g�̍X�V��L���ɂ���
	for (auto i : *(GetScene()->GetChildList()))
	{
		i->SetUpdate(true);
	}
	uiMode_ = UI_MODE::MODE_INVALID;
}

void PauseUI::Input()
{
	//�X�^�[�g�{�^�������ꂽ��Q�[���̍X�V���ĊJ
	if (Input::IsPadButtonDown(XINPUT_GAMEPAD_START))
	{
		isPause_ = false;
		GameResume();
	}

	if (isPause_)
	{
		//�{�^���̏���
		buttonMove_ = 0;
		//�E�Ɉړ�
		if (Input::GetLStick_X() >= STICK_TILT)
		{
			buttonMove_ = RIGHT;
			//�{�^�����Ō������Ȃ�������ړ����[�h�ɐ؂�ւ���
			if (buttonNum_ < (buttonCount_ - 1))
				uiMode_ = UI_MODE::MODE_MOVE;
		}
		//���Ɉړ�
		else if (Input::GetLStick_X() <= -STICK_TILT)
		{
			buttonMove_ = LEFT;
			//�{�^���̔ԍ����ŏ�����Ȃ�������ړ����[�h�ɐ؂�ւ�
			if (buttonNum_ > 0)
				uiMode_ = UI_MODE::MODE_MOVE;
		}
		//�{�^���̔ԍ��𒲐�
		buttonNum_ += buttonMove_;
		buttonNum_ = Clamp<float>((float)buttonNum_, 0.0f, (float)(buttonCount_ - 1));

		PushedButton(buttonNum_);
	}
}

void PauseUI::Invalid()
{
	//�X�^�[�g�{�^����������UI���[�h�ɂ��ăQ�[���̍X�V���~�߂�
	if (Input::IsPadButtonDown(XINPUT_GAMEPAD_START))
	{
		uiMode_ = UI_MODE::MODE_INPUT;
		isPause_ = true;
		GameStop();
	}
}

void PauseUI::Move()
{
	moveTime_++;
	moveTime_ = min(moveTime_, MAX_MOVE_TIME);
	//�����I�������
	if (moveTime_ >= MAX_MOVE_TIME)
	{
		uiMode_ = UI_MODE::MODE_INPUT;
		moveTime_ = 0;
		//���������Z���N�g��ʂ̈ʒu���m�肳����
		framePos_.x += MOVE * buttonMove_;
		return;
	}
	//�Z���N�g��ʂ𓮂���
	MoveButton(Easing::EaseInCubic((float)moveTime_ / (float)(MAX_MOVE_TIME - 1)));
}

void PauseUI::Selected()
{

}

void PauseUI::ReadFile(std::string fileName)
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

void PauseUI::ThirdDraw()
{
	if (isPause_)
	{
		ImageManager::Draw(hPictBackGround_);
		ImageManager::Draw(hPictButtonFrame_);
		for (auto& i : buttonList_)
		{
			i.buttonText_->Draw();
		}
	}
}

void PauseUI::PushedButton(int num)
{
	if (Input::IsPadButtonDown(XINPUT_GAMEPAD_A))
	{
		switch (num)
		{
		case TITLE:
			((SceneManager*)FindObject("SceneManager"))->ChangeScene(SCENE_ID::SCENE_ID_SELECT, DELAY);
			break;
		case PLAY:
			((SceneManager*)FindObject("SceneManager"))->ChangeScene(SCENE_ID::SCENE_ID_PLAY, DELAY);
			break;
		}
		uiMode_ = UI_MODE::MODE_SELECTED;
	}
}

void PauseUI::LoadImageFile()
{
	//�|�[�Y��ʔw�i�摜�ǂݍ���
	hPictBackGround_ = ImageManager::Load(BACKGROUND_IMAGE);
	ImageManager::SetAlpha(hPictBackGround_, 0.2f);
	assert(hPictBackGround_ >= 0);
	//�{�^���̃t���[���摜�ǂݍ���
	hPictButtonFrame_ = ImageManager::Load(BUTTONFLAME_IMAGE);
	assert(hPictButtonFrame_ >= 0);

	ImageManager::SetImagePos(hPictButtonFrame_, BUTTON_FIRST_POS);
	//�{�^���摜�ǂݍ���
	for (auto elem : fileReader_[0][BUTTON_LIST_NAME].items().begin().value())
	{
		buttonCount_++;
		//�摜�̖��O���擾
		std::string button = elem[1];

		TEXT_RECT rect;
		rect = { elem[2][0],elem[2][1],elem[2][2] ,elem[2][3] };
		XMFLOAT3 pos = { elem[3][0],elem[3][1] ,elem[3][2] };
		/////////////////////////////�{�^���̗p��//////////////////////
		button_ btn;

		//�e�L�X�g�ǂݍ���
		btn.buttonText_ = new Text;
		btn.buttonText_->Load(button, "Sitka Text", rect, LEFT_TOP);


		//�ʒu�ݒ�
		btn.position_ = pos;
		btn.buttonText_->SetPosition({ btn.position_.x - btn.buttonText_->GetRect().right,
									   btn.position_.y + btn.buttonText_->GetRect().bottom });
		//�z��ɓ����
		buttonList_.push_back(btn);
	}
}

void PauseUI::Release()
{
}
