#include "ObjectSetter.h"
#include"Engine/Model.h"
#include"Engine/Fbx.h"
#include"Player.h"
#include"TitleUI.h"
#include"EnemyNormal.h"
#include"EnemyTurret.h"
#include"EnemyBoss.h"
#include"SetObject.h"
#include"Test.h"
#include"Stage1.h"
#include"StateList.h"

ObjectSetter::ObjectSetter(GameObject* parent)
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
		State::StateCreate();
		Instantiate<Stage1>(GetParent());
		Instantiate<Player>(GetParent());
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
	/*for (auto itr = enemys_.begin(); itr != enemys_.end(); )
	{
		if ((*itr)->GetLife() < 0)
		{
			itr = enemys_.erase(itr);
		}
		else
			itr++;
	}
	if (enemys_.empty())
	{
		enemys_.push_back(Instantiate<EnemyBoss>(GetParent()));
	}*/
	
}

void ObjectSetter::Draw()
{
}

void ObjectSetter::Release()
{
}
