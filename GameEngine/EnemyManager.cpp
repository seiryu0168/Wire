#include "EnemyManager.h"
#include"EnemyBoss.h"
#include"EnemyNormal.h"
#include"EnemyTurret.h"
#include<d3d11.h>
#include<fstream>
#include<sstream>
EnemyManager::EnemyManager()
{
}

EnemyManager::~EnemyManager()
{
	SAFE_DELETE(enemyData_);
	SAFE_RELEASE(pObject_);
}

void EnemyManager::Initialize(std::string fileName)
{
	//カレントディレクトリ取得
	WCHAR currentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, currentDir);

	WCHAR path[FILENAME_MAX];

	if (SetCurrentDirectory(L"Assets") == ERROR_FILE_NOT_FOUND)
	{
		return;
	}
	std::ifstream fileReader;
	fileReader.open(fileName.c_str());
	if (fileReader.good())
	{
		enemyData_ = new json;
		fileReader >> (*enemyData_);
	}
	fileReader.close();
	SetCurrentDirectory(currentDir);
}

void EnemyManager::SetParentObject(GameObject* obj)
{
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
	Enemy* pEnemy;
	XMFLOAT3 position;
	XMFLOAT3 rotate;
	XMFLOAT3 scale;
	//jsonファイルの名前から生成するエネミーを分岐し、エネミーリストに追加
	for (auto& elem : enemyData_[0].items())
	{
		pEnemy = InstantiateEnemy(elem.value().get<std::string>());
		//pEnemy->LoadModel(elem.value()["ModelName"]);
		position = { elem.value()["Position"][0],
					 elem.value()["Position"][1],
					 elem.value()["Position"][2] };
		rotate	 = { elem.value()["Rotate"][0],
					 elem.value()["Rotate"][1],
					 elem.value()["Rotate"][2] };
		scale	 = { elem.value()["Scale"][0],
					 elem.value()["Scale"][1],
					 elem.value()["Scale"][2] };
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

Enemy* EnemyManager::InstantiateEnemy(std::string enemyName)
{

	if (enemyName == "EnemyNormal")
	{
		return pObject_->Instantiate<EnemyNormal>(pObject_->GetScene());
	}
	else if (enemyName == "EnemyTurret")
	{
		return pObject_->Instantiate<EnemyTurret>(pObject_->GetScene());
	}
	else if (enemyName == "EnemyBoss")
	{
		return pObject_->Instantiate<EnemyBoss>(pObject_->GetScene());
	}
}
