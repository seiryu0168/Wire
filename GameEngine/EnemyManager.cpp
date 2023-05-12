#include "EnemyManager.h"
#include"EnemyBoss.h"
#include"EnemyNormal.h"
#include"EnemyTurret.h"
#include<d3d11.h>
#include<fstream>
#include<sstream>
namespace
{
	static const std::string INITIAL_ENEMY_STATUS = "InitialEnemyStatus";
}
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
		*enemyData_ = json::parse(fileReader);
		//fileReader >> (*enemyData_);
	}
	fileReader.close();
	SetCurrentDirectory(currentDir);
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
	Enemy* pEnemy;
	XMFLOAT3 position;
	XMFLOAT3 rotate;
	XMFLOAT3 scale;
	//jsonファイルの名前から生成するエネミーを分岐し、エネミーリストに追加
	for (auto& elem : enemyData_[0][INITIAL_ENEMY_STATUS].items())
	{
		auto enemyName = elem.value().items().begin().key();
		//auto a2 = a.begin().key();
		//auto b = a.items().begin().value()["EnemyName"].get<std::string>();
		pEnemy = InstantiateEnemy(enemyName);
		auto jPos = elem.value()[enemyName].items().begin().value()["Position"];
		auto jRotate= elem.value()[enemyName].items().begin().value()["Rotate"];
		auto jScale= elem.value()[enemyName].items().begin().value()["Scale"];
		//pEnemy->LoadModel(elem.value()["ModelName"]);
		position = { jPos[0],    jPos[1],	 jPos[2] };
		rotate	 = { jRotate[0], jRotate[1], jRotate[2] };
		scale	 = { jScale[0],	 jScale[1],	 jScale[2] };
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
		//GameObject* ob = pObject_->GetScene();
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
}
