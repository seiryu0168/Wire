#pragma once
#include"Enemy.h"
#include"EnemyState.h"
class TutorialEnemy : public Enemy
{
	int hModel_;
public:
	TutorialEnemy(GameObject* parent);
	~TutorialEnemy();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

