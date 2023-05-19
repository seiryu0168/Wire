#include "EnemyManager.h"
#include"EnemyBoss.h"
#include"EnemyNormal.h"
#include"EnemyTurret.h"
#include"EnemyTutorial.h"
#include<d3d11.h>
#include<fstream>
#include<sstream>
namespace
{
	static const std::string INITIAL_ENEMY_STATUS = "InitialEnemyStatus";
	static const std::string FILE_NAME[4] = { "TutorialStageEnemyStatus.json",
											  "Stage1EnemyStatus.json",
											  "Stage2EnemyStatus.json",
											  "Stage3EnemyStatus.json" };
}
EnemyManager::EnemyManager()
{
}

EnemyManager::~EnemyManager()
{
	SAFE_DELETE(enemyData_);
	SAFE_RELEASE(pObject_);
}

void EnemyManager::Initialize(int stageNum)
{
	//ステージ番号がファイルの最大数以上だったらファイルの最大数に直す
	int maxStageCount = sizeof(FILE_NAME) / sizeof(std::string);
	if (maxStageCount < stageNum)
		stageNum = maxStageCount - 1;
	bool loadSuccess = LoadFile(FILE_NAME[stageNum]);
	assert(loadSuccess);
}

bool EnemyManager::LoadFile(std::string fileName)
{
	//カレントディレクトリ取得
	WCHAR currentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, currentDir);

	WCHAR path[FILENAME_MAX];

	if (SetCurrentDirectory(L"Assets") == ERROR_FILE_NOT_FOUND)
	{
		return false;
	}
	std::ifstream fileReader;
	fileReader.open(fileName.c_str());
	if (fileReader.good() == false)
	{
		return false;
	}
		enemyData_ = new json;
		*enemyData_ = json::parse(fileReader);
		//fileReader >> (*enemyData_);
	fileReader.close();
	SetCurrentDirectory(currentDir);
	return true;
}

void EnemyManager::SetParentObject(GameObject* obj)
{
	pObject_ = obj;
}

void EnemyManager::Update()
{
	for (auto itr = enemyList_.begin(); itr != enemyList_.end(); )
	{
		if ((*itr)->GetLife() < 0)
		{
			itr = enemyList_.erase(itr);
		}
		else
			itr++;
	}
}

void EnemyManager::SetEnemy()
{
	XMFLOAT3 position;
	XMFLOAT3 rotate;
	XMFLOAT3 scale;
	//jsonファイルの名前から生成するエネミーを分岐し、エネミーリストに追加
	for (auto& elem : enemyData_[0][INITIAL_ENEMY_STATUS].items())
	{
		Enemy* pEnemy;
		//エネミーの名前取得
		auto enemyName = elem.value().items().begin().key();
		//エネミー生成
		pEnemy = InstantiateEnemy(enemyName);
		//座標、回転、拡縮の情報取得
		auto jPos = elem.value()[enemyName].items().begin().value()["Position"];
		auto jRotate= elem.value()[enemyName].items().begin().value()["Rotate"];
		auto jScale= elem.value()[enemyName].items().begin().value()["Scale"];
		
		//代入
		position = { jPos[0],    jPos[1],	 jPos[2] };
		rotate	 = { jRotate[0], jRotate[1], jRotate[2] };
		scale	 = { jScale[0],	 jScale[1],	 jScale[2] };
		//生成したエネミーに設定する
		pEnemy->SetPosition(position);
		pEnemy->SetRotate(rotate);
		pEnemy->SetScale(scale);
		enemyList_.push_back(pEnemy);
	}
}

bool EnemyManager::IsAllEnemyDestroy()
{
	if (enemyList_.size() <= 0)
	{
		return true;
	}
	return false;
}

bool EnemyManager::IsActiveEnemyDestroy()
{
	for (auto enemys : enemyList_)
	{
		if (enemys->IsActive())
		{
			return false;
		}
	}
	return true;
}

void EnemyManager::BootNotActiveEnemy()
{
	for (auto itr = enemyList_.begin(); itr != enemyList_.end(); itr++)
	{
		if ((*itr)->IsActive() == false)
		{
			(*itr)->SetActive(true);
		}
	}
}

Enemy* EnemyManager::InstantiateEnemy(std::string enemyName)
{

	if (enemyName == "EnemyNormal")
	{
		return pObject_->Instantiate<EnemyNormal>(pObject_);
	}
	else if (enemyName == "EnemyTurret")
	{
		return pObject_->Instantiate<EnemyTurret>(pObject_);
	}
	else if (enemyName == "EnemyBoss")
	{
		return pObject_->Instantiate<EnemyBoss>(pObject_);
	}
	else if (enemyName == "EnemyTutorial")
	{
		return pObject_->Instantiate<EnemyTutorial>(pObject_);
	}
}
