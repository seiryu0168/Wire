#include "ObjectSetter.h"
#include"Engine/ResourceManager/Model.h"
#include"Engine/DirectX_11/Fbx.h"
#include"Engine/ResourceManager/ImageManager.h"
#include"Player.h"
#include"TitleUI.h"
#include"EnemyNormal.h"
#include"EnemyTurret.h"
#include"EnemyBoss.h"
#include"SetObject.h"
#include"InterSceneData.h"
#include"Test.h"
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
}

ObjectSetter::~ObjectSetter()
{
}

void ObjectSetter::Initialize()
{
	pManager_ = (SceneManager*)FindObject("SceneManager");
	//オブジェクト設置クラスでエネミーのパラメータを設定するようにする
	//feildって名前にしといたほうがいい
	std::string parentName;
	//親のオブジェクト名を取得
	parentName = GetParent()->GetObjectName();

	//親がプレイシーンだったら
	if (parentName == "PlayScene")
	{
		//リザルトデータ削除
		InterSceneData::DeleteData("Result");
		Instantiate<Stage1>(GetParent());
		pPlayer_ = Instantiate<Player>(GetParent());
		for (int i = 0; i < 3; i++)
		{
			enemys_.push_back(Instantiate<EnemyNormal>(GetParent()));
		}

		enemys_.push_back(Instantiate<EnemyTurret>(GetParent()));
	}
	//親がタイトルシーンだったら
	if (parentName == "TitleScene")
	{
		Instantiate<TitleUI>(GetParent());
	}
	if (parentName == "TutorialScene")
	{
		Instantiate<Stage1>(GetParent());
		Instantiate<Player>(GetParent());
	}
	hPict_ = ImageManager::Load("Assets\\Black.png");
	assert(hPict_ >= 0);
	ImageManager::SetAlpha(hPict_,0.0f);
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
	//プレイヤーのライフが0になったら
	if (pPlayer_->GetLife() <= 0)
	{
		if (resultFrag_ == false)
		{
			resultFrag_ = true;
			bool result = false;
			InterSceneData::AddData("Result", nullptr, nullptr, nullptr, &result);
		}
		if(!(pManager_->GetNextSceneID() == (int)SCENE_ID::SCENE_ID_RESULT))
		pManager_->ChangeScene((int)SCENE_ID::SCENE_ID_RESULT, DELAY);

		ImageManager::SetAlpha(hPict_, (float)(DELAY - pManager_->GetCountDown()) / (float)DELAY);
	}
	//エネミーが全て倒されたらボス出現
	if (enemys_.empty() && bossSpawn_ == false)
	{
		enemys_.push_back(Instantiate<EnemyBoss>(GetParent()));
		bossSpawn_ = true;
	}
	//ボス含めてエネミーがすべて倒されたら
	else if (bossSpawn_ && enemys_.empty())
	{
		if (resultFrag_ == false)
		{
			resultFrag_ = true;
			bool result = true;
			InterSceneData::AddData("Result", nullptr, nullptr, nullptr, &result);
		}
		if (!(pManager_->GetNextSceneID() == (int)SCENE_ID::SCENE_ID_RESULT))
		pManager_->ChangeScene((int)SCENE_ID::SCENE_ID_RESULT, DELAY);

		ImageManager::SetAlpha(hPict_, (float)(DELAY - pManager_->GetCountDown()) / (float)DELAY);
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
