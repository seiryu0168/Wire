#pragma once
#include "EnemyState.h"

class StateSearch : public EnemyState
{
private:

public:
	~StateSearch();
	void Init(Enemy*enemy) override;
	void Update(Enemy* enemy) override;
};

class StateChase : public EnemyState
{
	~StateChase();
	void Init(Enemy* enemy) override;
	void Update(Enemy* enemy) override;
};