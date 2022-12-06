#pragma once
#include "State.h"
#include"Player.h"
#include"EnemyNormal.h"
#include"Engine/GameObject.h"
class PlayerState : public State
{
private:
	Player* pPlayer_;
public:
	PlayerState(Player* player);
	~PlayerState();
	void Initialize() override;
	void Update() override;
};

class EnemyState : State
{private:
	Enemy* pEnemy_;
public:
	EnemyState(Enemy* enemy);
	~EnemyState();
	void Initialize() override;
	void Update() override;
};