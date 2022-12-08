#include "EnemyStateDef.h"
#include"EnemyNormal.h"
#include"StateList.h"

////////////////////////////////////////////õ“Gó‘Ô///////////////////////////////////////////
StateSearch::~StateSearch()
{

}

void StateSearch::Init(Enemy* enemy)
{
	
}
void StateSearch::Update(Enemy* enemy)
{
	if (enemy->IsVisible(enemy->GetViewAngle(), enemy->GetViewRange()))
	{

		enemy->ChangeState(State::chase->GetInstance());

	}
}

////////////////////////////////////////////’ÇÕó‘Ô//////////////////////////////////////////
StateChase::~StateChase()
{

}

void StateChase::Init(Enemy* enemy)
{

}
void StateChase::Update(Enemy* enemy)
{
	enemy->Attack();
	if (!(enemy->IsVisible(enemy->GetViewAngle(), enemy->GetViewRange())))
	{

		enemy->ChangeState(State::search->GetInstance());

	}
}