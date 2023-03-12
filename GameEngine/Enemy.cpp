#include "Enemy.h"
#include"Engine/ResourceManager/Model.h"
//コンストラクタ
Enemy::Enemy(GameObject* parent, std::string name)
	:GameObject(parent, name)
{
	
	enemyParameter_.life = 3;
	enemyParameter_.toPlayerVec = XMVectorSet(0, 0, 0, 0);
	enemyParameter_.frontVec = XMVectorSet(0, 0, 1, 0);
	enemyParameter_.upVec = XMVectorSet(0, 1, 0, 0);
	enemyParameter_.matX = XMMatrixIdentity();
	enemyParameter_.matY; XMMatrixIdentity();
	enemyParameter_.visibleFlag = false;
	enemyParameter_.isTargetList = false;
	enemyParameter_.vPosition = XMLoadFloat3(&transform_.position_);
	enemyParameter_.pPlayer = nullptr;
}

//デストラクタ
Enemy::~Enemy()
{

}

bool Enemy::IsVisible( float visibleAngle, float range)
{
	XMVECTOR toPlayer;
	XMFLOAT3 playerPos = GetPlayerPointer()->GetPosition();
	enemyParameter_.toPlayerVec = XMLoadFloat3(&playerPos) - enemyParameter_.vPosition;
	float toPlayerRange;
	toPlayerRange = VectorLength(enemyParameter_.toPlayerVec); //視界判定用の視界の長さ取得
	toPlayer = XMVector3Normalize(enemyParameter_.toPlayerVec);					//正規化

	//内積を求めて角度計算(1以上にならないようmin関数つけた)
	float angle = acosf(min(VectorDot(GetFrontVec(), toPlayer), 1));
	if (toPlayerRange <= 2 * range)
	{
		enemyParameter_.pPlayer->AddTargetList(this);
		enemyParameter_.visibleFlag = true;
		if (angle<visibleAngle && angle>-visibleAngle && toPlayerRange < range)
		{
			TurnToPlayer(toPlayer);
			
			return true;
		}
	}
	else
	{
		enemyParameter_.visibleFlag = false;
	}

	return false;
}

void Enemy::TurnToPlayer(XMVECTOR vToPlayer)
{
	//y軸の値を0にして平面のベクトルとして考える
	XMFLOAT3 buff(0, 0, 0);
	XMStoreFloat3(&buff, vToPlayer);
	buff.y = 0;
	vToPlayer=XMLoadFloat3(&buff);
	XMVECTOR base = XMVectorSet(0, 0, 1, 0);
	XMVECTOR fVec = XMVectorSet(0, 0, 0, 0);
	XMStoreFloat3(&buff, GetFrontVec());
	buff.y = 0;
	fVec = XMLoadFloat3(&buff);
	float angle = XMVectorGetX(XMVector3Dot(base, fVec));
	XMStoreFloat3(&buff, XMVector3Cross(base, fVec));
	
	angle = acosf(min(angle, 1));
	if (buff.y < 0)
	{
		angle+=M_PI*2;
		angle *= -1;
	}
	enemyParameter_.frontVec = vToPlayer;
	transform_.rotate_.y = (180.0f/M_PI)*angle;
}

bool Enemy::IsLockOned(Enemy* enemy)
{
	if (GetPlayerPointer()->GetTargetEnemyNum() == -1)
		return false;
	else if (GetPlayerPointer()->GetTargetEnemyNum() == enemy->GetObjectID())
		return true;
}

void Enemy::LoadModel(std::string fileName)
{
	//hModel_ = ModelManager::Load("Assets\\" + fileName);
}
