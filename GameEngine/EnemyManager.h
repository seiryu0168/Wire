#pragma once
#include"Engine/ResourceManager/json.hpp"
#include"Engine/GameObject/GameObject.h"

class Enemy;
class EnemyManager
{
private:
	GameObject* pObject_;
	std::vector<Enemy*> enemyList_;
public:
	EnemyManager();
	~EnemyManager();
	void SetParentObject(GameObject* obj);
	void LoadEnemyData(std::string );
	void SetEnemy();
};

