#include "EnemyTutorial.h"
#include"Engine/ResourceManager/Model.h"
#include"Engine/Collider/SphereCollider.h"
EnemyTutorial::EnemyTutorial(GameObject* parent)
	:Enemy(parent,"TutorialEnemy"),
	hModel_(-1)
{
}

EnemyTutorial::~EnemyTutorial()
{
}

void EnemyTutorial::Initialize()
{
	SetTag("Enemy");
	hModel_ = ModelManager::Load("Assets\\EnemyTurret.fbx");
	assert(hModel_ >= 0);
	SethModel(hModel_);
	ModelManager::SetModelNum(hModel_);
	SphereCollider* pCollision = new SphereCollider({ 0,0,0 }, 2);
	AddCollider(pCollision);
	SetPlayerPointer((Player*)FindObject("Player"));
}

void EnemyTutorial::Update()
{
	IsVisible(sight.angle_,sight.range_);
}

void EnemyTutorial::Draw()
{
	ModelManager::SetTransform(hModel_, transform_);
	if (IsLockOned(this))
		ModelManager::DrawOutLine(hModel_, { 1,0,0,1 });
	else if (GetPlayerPointer()->IsAim())
		ModelManager::DrawOutLine(hModel_, { 1,1,0,1 });
	else
		ModelManager::Draw(hModel_);
}

void EnemyTutorial::Release()
{
}
