#include "Enemy.h"
#include"Engine/ResourceManager/Model.h"
namespace
{
	static const XMVECTOR UPVEC= XMVectorSet(0, 0, 1, 0);
}
//コンストラクタ
Enemy::Enemy(GameObject* parent, std::string name)
	:GameObject(parent, name)
{
	
	enemyParameter_.life = 3;
	enemyParameter_.toPlayerVec = XMVectorZero();
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
	float angle = acosf(Clamp(VectorDot(GetFrontVec(), toPlayer),-1, 1));
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
	
	//向きベクトルを再設定
	vToPlayer=XMLoadFloat3(&buff);
	
	//外積出すためのベクトルを作る
	XMVECTOR base = UPVEC;
	XMVECTOR fVec = XMVectorZero();
	fVec = GetFrontVec();
	
	XMStoreFloat3(&buff, XMVector3Cross(base, fVec));
	
	float angle = VectorDot(base, fVec);
	angle = acosf(Clamp(angle,-1, 1));
	//buff.yが負の値だったら
	if (IsNegative(buff.y))
	{
		angle *= -1;
	}
	SetFrontVec(vToPlayer);
	transform_.rotate_.y = XMConvertToDegrees(angle);
}

bool Enemy::IsLockOned(Enemy* enemy)
{
	//プレイヤーがターゲットを捕捉していなかったら
	if (GetPlayerPointer()->GetTargetEnemyNum() == -1)
		return false;
	else if (GetPlayerPointer()->GetTargetEnemyNum() == enemy->GetObjectID())
		return true;
}

void Enemy::LoadModel(std::string fileName)
{
	//hModel_ = ModelManager::Load("Assets\\" + fileName);
}
