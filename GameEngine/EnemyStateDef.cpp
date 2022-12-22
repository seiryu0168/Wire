#include "EnemyStateDef.h"
#include"EnemyNormal.h"
#include"StateList.h"

//ステータスを定義するクラス


////////////////////////////////////////////索敵状態///////////////////////////////////////////
StateSearch::~StateSearch()
{

}

void StateSearch::Init(Enemy* enemy)
{
	
}
void StateSearch::Update(Enemy* enemy)
{
	
	if (enemy->IsVisible(enemy->GetViewAngle(), enemy->GetViewRange()))
	{//エネミーがプレイヤーを見つけたらステータスを変える

		enemy->ChangeState(State::chase->GetInstance());

	}
}

////////////////////////////////////////////追跡状態//////////////////////////////////////////
StateChase::~StateChase()
{

}

void StateChase::Init(Enemy* enemy)
{

}
void StateChase::Update(Enemy* enemy)
{
	//攻撃
	enemy->Attack();
	if (!(enemy->IsVisible(enemy->GetViewAngle(), enemy->GetViewRange())))
	{

		enemy->ChangeState(State::search->GetInstance());

	}
}