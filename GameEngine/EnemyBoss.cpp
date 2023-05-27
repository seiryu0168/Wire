#include "EnemyBoss.h"
//#include"Engine/ResourceManager/Model.h"
#include"ModelComponent.h"
#include"Bullet.h"
#include"Pointer.h"
#include"Engine/Collider/BoxCollider.h"
#include"HomingBullet.h"
namespace
{
	static const float RANGE_NEAR = 75.0f;
	static const float RANGE_FAR = 125.0f;
	static const int SECOND_MODE_LIFE = 3;
	static const float SEARCH_ANGLE = 360.0f;
	static const float SEARCH_RANGE = 200.0f;
	static const int LIFE = 5;
	static const XMFLOAT3 COLLISION_SIZE = { 5, 10, 5 };
	static const XMFLOAT4 COLOR_RED = { 1,0,0,1 };
	static const XMFLOAT4 COLOR_YELLOW = { 1,1,0,1 };
	static const std::string CORE_MODEL = "Assets\\Model\\EnemyBossCore.fbx";
	static const std::string SHIELD_MODEL = "Assets\\Model\\EnemyBossShield.fbx";

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
	SetActive(false);

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
	BoxCollider* pCollider = new BoxCollider({0,0,0}, COLLISION_SIZE);
	AddCollider(pCollider);
	
	//�R�A�̃��f���ǂݍ���
	hModelCore_ = ModelManager::Load(CORE_MODEL);
	ModelManager::SetModelNum(hModelCore_);
	assert(hModelCore_ >= 0);
	//�V�[���h�̃��f���ǂݍ���
	hModelShield_ = ModelManager::Load(SHIELD_MODEL);
	assert(hModelShield_ >= 0);
	SethModel(hModelShield_);
	ModelManager::SetModelNum(hModelShield_);
	
	//���o�̐ݒ�
	sight.SetAngle(XMConvertToRadians(SEARCH_RANGE));
	sight.SetRange(SEARCH_RANGE);
	
	//HP�ݒ�
	SetLife(LIFE);
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
	//���b�N�I������Ă���
	if (IsLockOned(this))
		ModelManager::DrawOutLine(hModelCore_, COLOR_RED);
	//���b�N�I������ĂȂ����A�v���C���[���G�C�����[�h�ɂȂ��Ă���
	else if (GetPlayerPointer()->IsAim())
		ModelManager::DrawOutLine(hModelCore_, COLOR_YELLOW);
	//�����Ȃ�
	else
		ModelManager::Draw(hModelCore_);
	//HP��3�ȏ�Ȃ�V�[���h��\��
	if (GetLife() >= SECOND_MODE_LIFE)
	{
		//��Ɠ���
			ModelManager::SetTransform(hModelShield_, transform_);
		if (IsLockOned(this))
			ModelManager::DrawOutLine(hModelShield_, COLOR_RED);
		else if (GetPlayerPointer()->IsAim())
			ModelManager::DrawOutLine(hModelShield_, COLOR_YELLOW);
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
				//GetComponent<ModelComponent>()->SetActive(false);
				ModelManager::DeleteModelNum(hModelCore_);
				SetIsList(false);
				KillMe();
			}
			else if (GetLife() < SECOND_MODE_LIFE)
			{
				ChangeState(StateSecondMode::GetInstance());
				//GetComponent<ModelComponent>(1)->SetActive(false);
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