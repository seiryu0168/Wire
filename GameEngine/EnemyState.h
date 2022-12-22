#pragma once
class Enemy;

//ステータスの親クラス
class EnemyState
{
public:
	virtual ~EnemyState() {};
	virtual void Init(Enemy* enemy)=0;
	virtual void Update(Enemy* enemy)=0;
};

