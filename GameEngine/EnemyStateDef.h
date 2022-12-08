#pragma once
#include "EnemyState.h"

class StateSearch : public EnemyState
{
public:
	~StateSearch();
	static StateSearch* GetInstance()
	{
		static StateSearch instance;
		return &instance;
	}
	void Init(Enemy*enemy) override;
	void Update(Enemy* enemy) override;
};

class StateChase : public EnemyState
{
public:
	~StateChase();
	static StateChase* GetInstance()
	{
		static StateChase instance;
		return &instance;
	}
	void Init(Enemy* enemy) override;
	void Update(Enemy* enemy) override;
};