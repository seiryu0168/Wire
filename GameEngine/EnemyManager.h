#pragma once
#include"Engine/ResourceManager/json.hpp"
#include"Engine/GameObject/GameObject.h"

class Enemy;
class EnemyManager
{
	using json = nlohmann::json;
private:
	GameObject* pObject_;
	json* enemyData_;
	std::list<Enemy*> enemyList_;
public:
	EnemyManager();
	~EnemyManager();
	void Initialize(int stageNum);
	bool LoadFile(std::string fileName);
	void SetParentObject(GameObject* obj);
	void Update();
	void SetEnemy();
	int EnemyCount() { return (int)enemyList_.size(); }
	bool IsAllEnemyDestroy();
	bool IsActiveEnemyDestroy();
	void BootNotActiveEnemy();
	std::list<Enemy*>* GetEnemyList() {return &enemyList_; }
	Enemy* InstantiateEnemy(std::string enemyName);
};

