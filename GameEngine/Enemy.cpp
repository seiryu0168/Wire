#include "Enemy.h"
#include"Engine/ResourceManager/Model.h"
//�R���X�g���N�^
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

//�f�X�g���N�^
Enemy::~Enemy()
{

}

bool Enemy::IsVisible( float visibleAngle, float range)
{
	XMVECTOR toPlayer;
	XMFLOAT3 playerPos = GetPlayerPointer()->GetPosition();
	enemyParameter_.toPlayerVec = XMLoadFloat3(&playerPos) - enemyParameter_.vPosition;
	float toPlayerRange;
	toPlayerRange = XMVectorGetX(XMVector3Length(enemyParameter_.toPlayerVec));			//���E����p�̎��E�̒����擾
	toPlayer = XMVector3Normalize(enemyParameter_.toPlayerVec);							//���K��

	XMVECTOR dot = XMVector3Dot(GetFrontVec(), toPlayer);								//���ς��v�Z
	float angle = acos(min(XMVectorGetX(dot), 1));										//�p�x�v�Z(1�ȏ�ɂȂ�Ȃ��悤min�֐�����)
	if (toPlayerRange <= 2 * range)
	{
		enemyParameter_.pPlayer->AddTargetList(this);
		enemyParameter_.visibleFlag = true;
		if (angle<visibleAngle && angle>-visibleAngle && toPlayerRange < range)
		{
			enemyParameter_.frontVec = toPlayer;
			return true;
		}
	}
	else
	{
		enemyParameter_.visibleFlag = false;
	}

	return false;
}

void Enemy::LoadModel(std::string fileName)
{
	hModel_ = ModelManager::Load("Assets\\" + fileName);
}
