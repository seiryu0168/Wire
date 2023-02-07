#include "ObjectSetter.h"
#include"Engine/Model.h"
#include"Engine/Fbx.h"
#include"PlayerTest.h"
#include"Engine//SceneManager.h"
#include"TitleUI.h"
#include"EnemyNormal.h"
#include"EnemyTurret.h"
#include"EnemyBoss.h"
#include"SetObject.h"
#include"InterSceneData.h"
#include"Test.h"
#include"Stage1.h"

ObjectSetter::ObjectSetter(GameObject* parent)
	:GameObject(parent,"ObjectSetter")
{
}

ObjectSetter::~ObjectSetter()
{
}

void ObjectSetter::Initialize()
{
	//オブジェクト設置クラスでエネミーのパラメータを設定するようにする
	//feildって名前にしといたほうがいい
	std::string parentName;
	parentName = GetParent()->GetObjectName();
	if (parentName == "PlayScene")
	{
		Instantiate<Stage1>(GetParent());
		Instantiate<PlayerTest>(GetParent());
		for (int i = 0; i < 3; i++)
		{
			enemys_.push_back(Instantiate<EnemyNormal>(GetParent()));
		}

		enemys_.push_back(Instantiate<EnemyTurret>(GetParent()));
	}

	if (parentName == "TitleScene")
	{
		Instantiate<TitleUI>(GetParent());
	}
}

void ObjectSetter::Update()
{
	switch ((SCENE_ID)((SceneManager*)GetParent()->GetParent())->GetCurrentSceneID())
	{
	case SCENE_ID::SCENE_ID_TITLE:
		TitleUpdate();
		break;

	case SCENE_ID::SCENE_ID_PLAY:
		PlayUpdate();
		break;
	default:
		break;

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
	if (enemys_.empty() && bossSpawn_ == false)
	{
		enemys_.push_back(Instantiate<EnemyBoss>(GetParent()));
		bossSpawn_ = true;
	}
	else if (bossSpawn_ && enemys_.empty())
	{
		bool result = true;
		InterSceneData::AddData("Result", nullptr, nullptr, nullptr, &result);
		((SceneManager*)FindObject("SceneManager"))->ChangeScene((int)SCENE_ID::SCENE_ID_RESULT);
	}
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
