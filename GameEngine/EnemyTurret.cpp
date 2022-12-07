#include "EnemyTurret.h"
#include"Engine/Model.h"
#include"Engine/SphereCollider.h"
//コンストラクタ
EnemyTurret::EnemyTurret(GameObject* parent)
	:Enemy(parent,"EnemyTurret"),
	targetingRange_(70.0f),
	viewangle_(90.0f)
{

}

//デストラクタ
EnemyTurret::~EnemyTurret()
{

}

//初期化
void EnemyTurret::Initialize()
{
	hModel_ = Model::Load("Assets\\Enemy2.fbx");
	assert(hModel_ >= 0);


}

//更新
void EnemyTurret::Update()
{

}

void EnemyTurret::FixedUpdate()
{

}
//描画
void EnemyTurret::Draw()
{

}

void EnemyTurret::Release()
{

}