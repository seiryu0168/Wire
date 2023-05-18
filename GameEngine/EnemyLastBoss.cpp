#include "EnemyLastBoss.h"

void EnemyLastBoss::ChangeState(EnemyState<EnemyLastBoss>* state)
{
	if (state != pState_)
	{
		pState_ = state;
		pState_->Init(*this);
	}
}

EnemyLastBoss::EnemyLastBoss(GameObject* parent)
	:Enemy(parent,"EnemyLastBoss")
{
}

EnemyLastBoss::~EnemyLastBoss()
{
}

void EnemyLastBoss::Initialize()
{
	//hModel_=ModelManager::Load()
	ChangeState(StateFirstMode::GetInstance());
}

void EnemyLastBoss::Update()
{
	pState_->Update(*this);
}

void EnemyLastBoss::Draw()
{
}
