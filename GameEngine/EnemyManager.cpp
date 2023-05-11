#include "EnemyManager.h"

EnemyManager::EnemyManager()
{
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::SetParentObject(GameObject* obj)
{
}

void EnemyManager::LoadEnemyData(std::string)
{
}

void EnemyManager::SetEnemy()
{
	pObject_->Instantiate<>()
}
