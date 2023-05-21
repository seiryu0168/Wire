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
	//オブジェクト設置クラスでエネミーのパラメータを設定するようにする

	int stageNum = InterSceneData::GetintData("StageNum");
	//親がプレイシーンだったら
	if (nowSceneID_ == SCENE_ID::SCENE_ID_PLAY)
	{
		//リザルトデータ削除
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
	//親がタイトルシーンだったら
	if (nowSceneID_ == SCENE_ID::SCENE_ID_TITLE)
	{
		pManager_->ChangeScene(SCENE_ID::SCENE_ID_SELECT);
	}
	//親がセレクトシーンだったら
	if (nowSceneID_ == SCENE_ID::SCENE_ID_SELECT)
	{
		Instantiate<SelectUI>(GetParent());
	}
	//親がリザルトシーンだったら
	if (nowSceneID_ == SCENE_ID::SCENE_ID_RESULT)
	{
		Instantiate<ResultUI>(GetParent());
	}
	//親がチュートリアルシーンだったら
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
	//プレイヤーのライフが0になったら
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
	//ボス含めてエネミーがすべて倒されたら
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
	//エネミーが全て倒されたらボス出現
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

