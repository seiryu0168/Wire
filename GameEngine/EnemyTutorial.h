#pragma once
#include"Enemy.h"
#include"EnemyState.h"
class EnemyTutorial : public Enemy
{
	int hModel_;
public:
	EnemyTutorial(GameObject* parent);
	~EnemyTutorial();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

