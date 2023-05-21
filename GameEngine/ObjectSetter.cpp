#include "ObjectSetter.h"
#include"Engine/ResourceManager/Model.h"
#include"Engine/DirectX_11/Fbx.h"
#include"Engine/ResourceManager/ImageManager.h"
#include"ResultUI.h"
#include"TutorialUI.h"
#include"Player.h"
#include"SelectUI.h"
#include"EnemyTutorial.h"
#include"EnemyNormal.h"
#include"EnemyTurret.h"
#include"EnemyBoss.h"
#include"SetObject.h"
#include"InterSceneData.h"
#include"Test.h"
#include"MissionUI.h"
#include"Stage1.h"
namespace 
{
	static const int DELAY = 180;
}

ObjectSetter::ObjectSetter(GameObject* parent)
	:GameObject(parent, "ObjectSetter"),
	pPlayer_(nullptr),
	countDown_(0),
	bossSpawn_(false),
	pManager_(nullptr),
	hPict_(-1)
{
	set_ = new SetObject(GetParent());
	set_->Load(GetParent()->GetObjectName()+".txt");
	pManager_ = (SceneManager*)FindObject("SceneManager");
	nowSceneID_ = pManager_->GetNextSceneID();
	//�I�u�W�F�N�g�ݒu�N���X�ŃG�l�~�[�̃p�����[�^��ݒ肷��悤�ɂ���

	int stageNum = InterSceneData::GetintData("StageNum");
	//�e���v���C�V�[����������
	if (nowSceneID_ == SCENE_ID::SCENE_ID_PLAY)
	{
		//���U���g�f�[�^�폜
		InterSceneData::DeleteData("Result");
		Stage1* p=Instantiate<Stage1>(GetParent());
		p->SetStageNum(stageNum);
		EManager_.SetParentObject(GetParent());
		EManager_.Initialize(stageNum);
		UManager_.SetParentObject(GetParent());
		UManager_.Initialize(stageNum);
		IManager_.SetParentObject(GetParent());
		IManager_.Initialize(stageNum);
		pPlayer_ = Instantiate<Player>(GetParent());
		EManager_.SetEnemy();
		UManager_.SetUI();
		IManager_.SetItem();
	}
	//�e���^�C�g���V�[����������
	if (nowSceneID_ == SCENE_ID::SCENE_ID_TITLE)
	{
		pManager_->ChangeScene(SCENE_ID::SCENE_ID_SELECT);
	}
	//�e���Z���N�g�V�[����������
	if (nowSceneID_ == SCENE_ID::SCENE_ID_SELECT)
	{
		Instantiate<SelectUI>(GetParent());
	}
	//�e�����U���g�V�[����������
	if (nowSceneID_ == SCENE_ID::SCENE_ID_RESULT)
	{
		Instantiate<ResultUI>(GetParent());
	}
	//�e���`���[�g���A���V�[����������
	if (nowSceneID_ == SCENE_ID::SCENE_ID_TUTORIAL)
	{
		Instantiate<Stage1>(GetParent());
		Instantiate<Player>(GetParent());
		EManager_.SetParentObject(GetParent());
		EManager_.Initialize(stageNum);
		EManager_.SetEnemy();
		Instantiate<TutorialUI>(GetParent());
	}
	//set_->ObjectSet();
}

ObjectSetter::~ObjectSetter()
{
	SAFE_DELETE(set_);
}

void ObjectSetter::Initialize()
{
	hPict_ = ImageManager::Load("Assets\\Black.png");
	assert(hPict_ >= 0);
	ImageManager::SetAlpha(hPict_,0.0f);
}

void ObjectSetter::Update()
{
	if (nowSceneID_ == (SCENE_ID)pManager_->GetNextSceneID())
	{

		switch ((SCENE_ID)pManager_->GetCurrentSceneID())
		{
		case SCENE_ID::SCENE_ID_TITLE:
			TitleUpdate();
			break;

		case SCENE_ID::SCENE_ID_PLAY:
			PlayUpdate();
			break;

		case SCENE_ID::SCENE_ID_TUTORIAL:
			TutorialUpdate();
			break;
		default:
			break;
		}
	}
	else
	{
		BlackOutUpdate();
	}
	
}

void ObjectSetter::Draw()
{
}

void ObjectSetter::ThirdDraw()
{
	ImageManager::Draw(hPict_);
}

void ObjectSetter::Release()
{
}

void ObjectSetter::TitleUpdate()
{
}

void ObjectSetter::PlayUpdate()
{
	EManager_.Update();
	//�v���C���[�̃��C�t��0�ɂȂ�����
	if (pPlayer_->GetLife() <= 0)
	{
		if (resultFrag_ == false)
		{
			resultFrag_ = true;
			bool result = false;
			InterSceneData::AddData("Result", nullptr, nullptr, nullptr, &result);
		}
		if(!(pManager_->GetNextSceneID() == SCENE_ID::SCENE_ID_RESULT))
		pManager_->ChangeScene(SCENE_ID::SCENE_ID_RESULT, DELAY);

		
	}
	//�{�X�܂߂ăG�l�~�[�����ׂē|���ꂽ��
	if(EManager_.IsAllEnemyDestroy())
	{
		if (resultFrag_ == false)
		{
			resultFrag_ = true;
			bool result = true;
			InterSceneData::AddData("Result", nullptr, nullptr, nullptr, &result);
		}
		if (!(pManager_->GetNextSceneID() == SCENE_ID::SCENE_ID_RESULT))
		pManager_->ChangeScene(SCENE_ID::SCENE_ID_RESULT, DELAY);

		//ImageManager::SetAlpha(hPict_, (float)(DELAY - pManager_->GetCountDown()) / (float)DELAY);
	}
	//�G�l�~�[���S�ē|���ꂽ��{�X�o��
	else if (EManager_.IsActiveEnemyDestroy())
	{
		EManager_.BootNotActiveEnemy();
	}
}

void ObjectSetter::TutorialUpdate()
{
}

void ObjectSetter::BlackOutUpdate()
{
	ImageManager::SetAlpha(hPict_, (float)(DELAY - pManager_->GetCountDown()) / (float)DELAY);
}

void ObjectSetter::GetEnemyList(std::list<Enemy*>* list)
{
	for (auto itr = EManager_.GetEnemyList()->begin(); itr != EManager_.GetEnemyList()->end(); itr++)
	{
		if ((*itr)->GetVisibleFrag())
		{
			list->push_back(*itr);
		}
	}
}

int ObjectSetter::GetEnemyCount()
{
	return EManager_.EnemyCount();
}

