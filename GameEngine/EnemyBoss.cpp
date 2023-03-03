#include "EnemyBoss.h"
#include"Engine/ResourceManager/Model.h"
#include"Bullet.h"
#include"Pointer.h"
#include"Engine/Collider/BoxCollider.h"
#include"HomingBullet.h"
namespace
{
	static const float nearRange = 75.0f;
	static const float farRange = 125.0f;
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
	SetTag("Enemy");
	BoxCollider* pCollider = new BoxCollider({0,0,0}, XMFLOAT3(5, 10, 5));
	AddCollider(pCollider);

	hModelCore_ = ModelManager::Load("Assets\\EnemyBossCore.fbx");
	ModelManager::SetModelNum(hModelCore_);
	assert(hModelCore_ >= 0);
	hModelShield_ = ModelManager::Load("Assets\\EnemyBossShield.fbx");
	ModelManager::SetModelNum(hModelShield_);
	assert(hModelShield_ >= 0);
	SetPlayerPointer((Player*)FindObject("Player"));
	sight.SetAngle((float)(M_PI*1.5f));
	sight.SetRange(200);
	
	SetLife(5);
	transform_.position_ = { 110,10,110 };
	ChangeState(StateSearch::GetInstance());
}

void EnemyBoss::Update()
{
	SetPositionVec(XMLoadFloat3(&transform_.position_));
	pState_->Update(*this);
}

void EnemyBoss::FixedUpdate()
{
}

void EnemyBoss::Draw()
{
	ModelManager::SetTransform(hModelCore_, transform_);
	if (IsLockOned(this))
		ModelManager::DrawOutLine(hModelCore_, { 1,0,0,1 });
	else if (GetPlayerPointer()->IsAim())
		ModelManager::DrawOutLine(hModelCore_, { 1,1,0,1 });
	else
		ModelManager::Draw(hModelCore_);
	
	if (GetLife() >= 3)
	{
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
	if (XMVectorGetX(XMVector3Length(GetToPlayerVec()))<nearRange)
	{
		//UŒ‚1
		//’¼i‚·‚é’e‚ÅUŒ‚
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
	else if(XMVectorGetX(XMVector3Length(GetToPlayerVec())) < farRange)
	{
		//UŒ‚2
		//’Ç”ö’e‚ÅUŒ‚
		HShot(true);
	}
}

void EnemyBoss::HShot(bool shot)
{
	shotTime_++;
	if (shot)
	{
		float isShot = 3600.0f / rpm_;
		if (isShot <= (float)shotTime_)
		{
			XMVECTOR shotDir = XMVector3Normalize(GetToPlayerVec());
			HomingBullet* pHBullet = Instantiate<HomingBullet>(this);
			shotTime_ = 0;
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
		if (isShot <= (float)shotTime_)
		{
			XMVECTOR shotDir = XMVector3Normalize(GetToPlayerVec());
			Bullet* pBullet = Instantiate<Bullet>(this);
			pBullet->SetDir(shotDir);
			shotTime_ = 0;
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