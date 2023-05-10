#include "Enemy.h"
#include"Engine/ResourceManager/Model.h"
namespace
{
	static const XMVECTOR UPVEC= XMVectorSet(0, 0, 1, 0);
}
//�R���X�g���N�^
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
	toPlayerRange = VectorLength(enemyParameter_.toPlayerVec); //���E����p�̎��E�̒����擾
	toPlayer = XMVector3Normalize(enemyParameter_.toPlayerVec);					//���K��

	//���ς����߂Ċp�x�v�Z(1�ȏ�ɂȂ�Ȃ��悤min�֐�����)
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
	//y���̒l��0�ɂ��ĕ��ʂ̃x�N�g���Ƃ��čl����
	XMFLOAT3 buff(0, 0, 0);
	XMStoreFloat3(&buff, vToPlayer);
	buff.y = 0;
	
	//�����x�N�g�����Đݒ�
	vToPlayer=XMLoadFloat3(&buff);
	
	//�O�Ϗo�����߂̃x�N�g�������
	XMVECTOR base = UPVEC;
	XMVECTOR fVec = XMVectorZero();
	fVec = GetFrontVec();
	
	XMStoreFloat3(&buff, XMVector3Cross(base, fVec));
	
	float angle = VectorDot(base, fVec);
	angle = acosf(Clamp(angle,-1, 1));
	//buff.y�����̒l��������
	if (IsNegative(buff.y))
	{
		angle *= -1;
	}
	SetFrontVec(vToPlayer);
	transform_.rotate_.y = XMConvertToDegrees(angle);
}

bool Enemy::IsLockOned(Enemy* enemy)
{
	//�v���C���[���^�[�Q�b�g��ߑ����Ă��Ȃ�������
	if (GetPlayerPointer()->GetTargetEnemyNum() == -1)
		return false;
	else if (GetPlayerPointer()->GetTargetEnemyNum() == enemy->GetObjectID())
		return true;
}

void Enemy::LoadModel(std::string fileName)
{
	//hModel_ = ModelManager::Load("Assets\\" + fileName);
}
