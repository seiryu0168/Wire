#include "PlayerEnemyState.h"
#include"Engine/Input.h"
#include"Engine/Model.h"
#include"Pointer.h"
PlayerState::PlayerState(Player* player)
{
	pPlayer_ = player;
}

void PlayerState::Initialize()
{
}

void PlayerState::Update(State& state)
{

}

EnemyState::EnemyState(Enemy* enemy)
	:EnemyState(enemy)
{
	pEnemy_ = enemy;
}

void EnemyState::Initialize()
{

}

void EnemyState::Update(State& state)
{

}