#include "ObjectSetter.h"
#include"Engine/ResourceManager/Model.h"
#include"Engine/DirectX_11/Fbx.h"
#include"Engine/ResourceManager/ImageManager.h"
#include"ResultUI.h"
#include"TutorialUI.h"
#include"Player.h"
#include"TitleUI.h"
#include"TutorialEnemy.h"
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
	pManager_ = (SceneManager*)FindObject("SceneManager");
	nowSceneID_ = pManager_->GetNextSceneID();
	//�I�u�W�F�N�g�ݒu�N���X�ŃG�l�~�[�̃p�����[�^��ݒ肷��悤�ɂ���
	//feild���Ė��O�ɂ��Ƃ����ق�������
	
	//�e�̃I�u�W�F�N�g�����擾
	sceneName_ = GetParent()->GetObjectName();

	//�e���v���C�V�[����������
	if (sceneName_ == "PlayScene")
	{

		//���U���g�f�[�^�폜
		InterSceneData::DeleteData("Result");
		Instantiate<Stage1>(GetParent());
		pPlayer_ = Instantiate<Player>(GetParent());
		for (int i = 0; i < 3; i++)
		{
			enemys_.push_back(Instantiate<EnemyNormal>(GetParent()));
		}

		enemys_.push_back(Instantiate<EnemyTurret>(GetParent()));
	}
	//�e���^�C�g���V�[����������
	if (sceneName_ == "TitleScene")
	{
		Instantiate<TitleUI>(GetParent());
	}
	//�e�����U���g�V�[����������
	if (sceneName_ == "ResultScene")
	{
		Instantiate<ResultUI>(GetParent());
	}
	//�e���`���[�g���A���V�[����������
	if (sceneName_ == "TutorialScene")
	{
		Instantiate<Stage1>(GetParent());
		Instantiate<Player>(GetParent());
		
		enemys_.push_back(Instantiate<TutorialEnemy>(GetParent()));
		Instantiate<TutorialUI>(GetParent());
	}
}

ObjectSetter::~ObjectSetter()
{
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

void ObjectSetter::Release()
{
}

void ObjectSetter::TitleUpdate()
{

}

void ObjectSetter::PlayUpdate()
{
	for (auto itr = enemys_.begin(); itr != enemys_.end(); )
	{
		if ((*itr)->GetLife() < 0)
		{
			itr = enemys_.erase(itr);
		}
		else
			itr++;
	}
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
	//�G�l�~�[���S�ē|���ꂽ��{�X�o��
	if (enemys_.empty() && bossSpawn_ == false)
	{
		enemys_.push_back(Instantiate<EnemyBoss>(GetParent()));
		bossSpawn_ = true;
	}
	//�{�X�܂߂ăG�l�~�[�����ׂē|���ꂽ��
	else if (bossSpawn_ && enemys_.empty())
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
	
	for (auto itr = enemys_.begin(); itr != enemys_.end(); itr++)
	{
		if ((*itr)->GetVisibleFrag())
		{
			list->push_back(*itr);
		}
	}
}

int ObjectSetter::GetEnemyCount()
{
	return enemys_.size();
}

