#include "Enemy.h"

//コンストラクタ
Enemy::Enemy(GameObject* parent, std::string name)
	:GameObject(parent, name)
{
	
	enemyParameter_.life = 5;
	enemyParameter_.toPlayerVec = XMVectorSet(0, 0, 0, 0);
	enemyParameter_.frontVec = XMVectorSet(0, 0, 1, 0);
	enemyParameter_.upVec = XMVectorSet(0, 1, 0, 0);
	enemyParameter_.matX = XMMatrixIdentity();
	enemyParameter_.matY; XMMatrixIdentity();
	enemyParameter_.visibleFlag = false;
	enemyParameter_.isTargetList = false;
	enemyParameter_.vPosition = XMVectorSet(0, 0, 0, 0);
	enemyParameter_.pPlayer = nullptr;

}

//デストラクタ
Enemy::~Enemy()
{

}


bool Enemy::IsVisible(XMVECTOR vFront, float visibleAngle, float range)
{

	XMVECTOR toPlayer;
	float rangeToPlayer;
	rangeToPlayer = XMVectorGetX(XMVector3Length(enemyParameter_.toPlayerVec));			//視界判定用の視界の長さ取得
	toPlayer = XMVector3Normalize(enemyParameter_.toPlayerVec);							//正規化

	XMVECTOR dot = XMVector3Dot(vFront, enemyParameter_.toPlayerVec);						//内積を計算
	float angle = acos(min(XMVectorGetX(dot), 1));						//角度計算(1以上にならないようmin関数つけた)
	if (rangeToPlayer <= 2 * range)
	{
		enemyParameter_.pPlayer->AddTargetList(this);
		enemyParameter_.isTargetList = true;
		if (angle<visibleAngle && angle>-visibleAngle && rangeToPlayer < range)
		{
			return true;
		}
	}
	else
	{
		enemyParameter_.isTargetList = false;
	}

	return false;
}
void Enemy::ChangeState(EnemyState* state)
{
	pEnemyState_ = state;
	pEnemyState_->Init(this);
}