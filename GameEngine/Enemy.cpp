#include "Enemy.h"

//コンストラクタ
Enemy::Enemy(GameObject* parent,std::string name)
	:GameObject(parent,name),
	life_(5),
	toPlayerVec_(XMVectorSet(0, 0, 0, 0)),
	frontVec_(XMVectorSet(0, 0, 1, 0)),
	upVec_(XMVectorSet(0, 1, 0, 0)),
	matX_(XMMatrixIdentity()),
	matY_(XMMatrixIdentity()),
	visibleFlag_(false),
	isTargetList_(false),
	vPosition_(XMVectorSet(0, 0, 0, 0)),
	pPlayer_(nullptr)
{

}

//デストラクタ
Enemy::~Enemy()
{

}


bool Enemy::IsVisible(XMVECTOR vFront, float visibleAngle, float range)
{

	XMVECTOR toPlayer;
	float rangeToPlayer;
	rangeToPlayer = XMVectorGetX(XMVector3Length(toPlayerVec_));			//視界判定用の視界の長さ取得
	toPlayer = XMVector3Normalize(toPlayerVec_);							//正規化

	XMVECTOR dot = XMVector3Dot(vFront, toPlayerVec_);						//内積を計算
	float angle = acos(min(XMVectorGetX(dot), 1));						//角度計算(1以上にならないようmin関数つけた)
	if (rangeToPlayer <= 2 * range)
	{
		pPlayer_->AddTargetList(this);
		isTargetList_ = true;
		if (angle<visibleAngle && angle>-visibleAngle && rangeToPlayer < range)
		{
			return true;
		}
	}
	else
	{
		isTargetList_ = false;
	}

	return false;
}
void Enemy::ChangeState(EnemyState* state)
{
	pEnemyState_ = state;
	pEnemyState_->Init(this);
}