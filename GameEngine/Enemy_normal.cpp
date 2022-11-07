#include "Enemy_normal.h"
#include"Engine/Model.h"
//コンストラクタ
Enemy_normal::Enemy_normal(GameObject* parent)
	:Enemy(parent,"Enemy_normal"),
	hModel_(-1),
	moveVec_(XMVectorSet(0,0,1,0)),
	matX_(XMMatrixIdentity()),
	matY_(XMMatrixIdentity()),
	pPlayer_(nullptr)
{

}

//デストラクタ
Enemy_normal::~Enemy_normal()
{

}

//初期化
void Enemy_normal::Initialize()
{
	hModel_ = Model::Load("Assets\\Box.fbx");
	pPlayer_ = (Player*)FindObject("Player");
}

//更新
void Enemy_normal::Update()
{

}

void Enemy_normal::FixedUpdate()
{

}
//描画
void Enemy_normal::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Enemy_normal::EnemyRotate(XMVECTOR toVec)
{
	XMVECTOR toVector;
	toVector = toVec - XMLoadFloat3(&transform_.position_);
}

//開放
void Enemy_normal::Release()
{

}