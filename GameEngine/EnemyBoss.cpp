#include "EnemyBoss.h"
//#include"Engine/ResourceManager/Model.h"
#include"Bullet.h"
#include"Pointer.h"
#include"Engine/Collider/BoxCollider.h"
#include"HomingBullet.h"
namespace
{
	static const float RANGE_NEAR = 75.0f;
	static const float RANGE_FAR = 125.0f;
}
EnemyBoss::EnemyBoss(GameObject* parent)
	:Enemy(parent,"EnemyBoss"),
	hModelCore_(-1),
	hModelShield_(-1),
	shotTime_(0),
	rpm_(200),
	perShot_(false),
	shotCount_(0),
	reLoadTime_(0),
	RELOAD(90)
{
}

EnemyBoss::~EnemyBoss()
{
}

void EnemyBoss::Initialize()
{
	//�^�O�ݒ�
	SetTag("Enemy");
	//�v���C���[�̃|�C���^�[�擾
	SetPlayerPointer((Player*)FindObject("Player"));
	//�����蔻��ݒ�
	BoxCollider* pCollider = new BoxCollider({0,0,0}, XMFLOAT3(5, 10, 5));
	AddCollider(pCollider);
	
	//�R�A�̃��f���ǂݍ���
	hModelCore_ = ModelManager::Load("Assets\\EnemyBossCore.fbx");
	ModelManager::SetModelNum(hModelCore_);
	assert(hModelCore_ >= 0);
	//�V�[���h�̃��f���ǂݍ���
	hModelShield_ = ModelManager::Load("Assets\\EnemyBossShield.fbx");
	SethModel(hModelShield_);
	ModelManager::SetModelNum(hModelShield_);
	assert(hModelShield_ >= 0);
	
	//���o�̐ݒ�
	sight.SetAngle((float)(M_PI*1.5f));
	sight.SetRange(200);
	
	//HP�ݒ�
	SetLife(5);
	//�ʒu�ݒ�
	transform_.position_ = { 110,10,110 };
	
	//�{���X�e�[�g�ɐݒ�
	ChangeState(StateSearch::GetInstance());
}

void EnemyBoss::Update()
{
	//���W�ݒ�
	SetPositionVec(XMLoadFloat3(&transform_.position_));
	pState_->Update(*this);
}

void EnemyBoss::FixedUpdate()
{
}

void EnemyBoss::Draw()
{
	//���W�ݒ�
	ModelManager::SetTransform(hModelCore_, transform_);
	//���b�N�I������Ă�Ȃ�
	if (IsLockOned(this))
		ModelManager::DrawOutLine(hModelCore_, { 1,0,0,1 });
	//Player��Aim���[�h�Ȃ�
	else if (GetPlayerPointer()->IsAim())
		ModelManager::DrawOutLine(hModelCore_, { 1,1,0,1 });
	//����ȊO
	else
		ModelManager::Draw(hModelCore_);
	//HP��3�ȏ�Ȃ�
	if (GetLife() >= 3)
	{
		//��Ɠ���
		ModelManager::SetTransform(hModelShield_, transform_);
		if (IsLockOned(this))
			ModelManager::DrawOutLine(hModelShield_, { 1,0,0,1 });
		else if (GetPlayerPointer()->IsAim())
			ModelManager::DrawOutLine(hModelShield_, { 1,1,0,1 });
		else
			ModelManager::Draw(hModelShield_);
	}
}

void EnemyBoss::Attack()
{
	//�v���C���[�Ƃ̋������߂�������
	if (XMVectorGetX(XMVector3Length(GetToPlayerVec()))< RANGE_NEAR)
	{
		//�U��1
		//���i����e�ōU��
		if (shotCount_ % 10 == 0 && perShot_)
		{
			reLoadTime_ = RELOAD;
			perShot_ = false;
		}
		else
		{
			reLoadTime_--;
			reLoadTime_ = max(reLoadTime_, 0);
		}

		if (reLoadTime_ == 0)
		{
			perShot_ = true;
		}

		Shot(perShot_);
	}
	//�v���C���[�Ƃ̋���������������
	else if(XMVectorGetX(XMVector3Length(GetToPlayerVec())) < RANGE_FAR)
	{
		//�U��2
		//�ǔ��e�ōU��
		HShot(true);
	}
}

void EnemyBoss::HShot(bool shot)
{
	shotTime_++;
	if (shot)
	{
		float isShot = 3600.0f / rpm_;
		//isShot��shotTime_�ȉ���������
		if (isShot <= (float)shotTime_)
		{
			//HomingBullet����
			HomingBullet* pHBullet = Instantiate<HomingBullet>(this);
			
			//shotTime_��0�ɂ���
			shotTime_ = 0;
			//��������
			shotCount_++;
		}
	}
}

void EnemyBoss::Shot(bool shot)
{
	shotTime_++;
	if (shot)
	{
		float isShot = 3600.0f / rpm_;
		//isShot��shotTime_�ȉ���������
		if (isShot <= (float)shotTime_)
		{
			//�ˌ��������v���C���[�̕����ɂ���
			XMVECTOR shotDir = XMVector3Normalize(GetToPlayerVec());
			//Bullet����
			Bullet* pBullet = Instantiate<Bullet>(this);
			//�ˌ������ݒ�
			pBullet->SetDir(shotDir);
			//shotTime_��0�ɂ���
			shotTime_ = 0;
			//��������
			shotCount_++;
		}
	}
}

void EnemyBoss::Release()
{
}

void EnemyBoss::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetTag() == "Player")
	{
		if (GetPlayerPointer()->GetSatatus() & ATC_ATTACK)
		{
			DecreaseLife(1);
			GetPlayerPointer()->SetStatus(ATC_DEFAULT);
			if (GetLife() < 0)
			{
				Transform pos;
				pos.position_ = { 9999,9999,9999 };
				ModelManager::DeleteModelNum(hModelCore_);
				SetIsList(false);
				KillMe();
			}
			else if (GetLife() < 3)
			{
				ChangeState(StateSecondMode::GetInstance());
				ModelManager::DeleteModelNum(hModelShield_);
				SethModel(hModelCore_);
			}
		}
	}
}

void EnemyBoss::StateChase::Init(EnemyBoss& enemy)
{
	enemy.shotTime_ = 0;
}

void EnemyBoss::StateChase::Update(EnemyBoss& enemy)
{
	enemy.Attack();
	if (!enemy.IsVisible(enemy.sight.angle_, enemy.sight.range_))
	{
		enemy.ChangeState(StateSearch::GetInstance());
	}
}

void EnemyBoss::StateSearch::Init(EnemyBoss& enemy)
{
}

void EnemyBoss::StateSearch::Update(EnemyBoss& enemy)
{
	if (enemy.IsVisible(enemy.sight.angle_, enemy.sight.range_))
	{
		enemy.ChangeState(StateChase::GetInstance());
	}
}

void EnemyBoss::StateSecondMode::Init(EnemyBoss& enemy)
{
	enemy.shotTime_ = 0;
}

void EnemyBoss::StateSecondMode::Update(EnemyBoss& enemy)
{

}

void EnemyBoss::ChangeState(EnemyState<EnemyBoss>* state)
{
	if (state != pState_)
	{
		pState_ = state;
		pState_->Init(*this);
	}
}