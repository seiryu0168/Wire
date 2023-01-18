#include "EnemyBoss.h"
#include"Engine/Model.h"
#include"Bullet.h"
#include"Pointer.h"
#include"HomingBullet.h"
namespace
{
	static const float nearRange = 75.0f;
	static const float farRange = 125.0f;
}
void EnemyBoss::ChangeState(EnemyState<EnemyBoss>* state)
{
	if (state != pState_)
	{
		pState_ = state;
		pState_->Init(*this);
	}
}
EnemyBoss::EnemyBoss(GameObject* parent)
	:Enemy(parent,"EnemyBoss"),
	hModel_(-1),
	shotTime_(0),
	rpm_(200)
{
}

EnemyBoss::~EnemyBoss()
{
}

void EnemyBoss::Initialize()
{
	SetTag("Enemy");
	OBBCollider* pCollider = new OBBCollider(XMFLOAT3(7, 11, 7), false, false);
	AddCollider(pCollider);

	hModel_ = ModelManager::Load("Assets\\EnemyBoss.fbx");
	ModelManager::SetModelNum(hModel_);
	assert(hModel_ >= 0);
	SetPlayerPointer((Player*)FindObject("Player"));
	sight.SetAngle(M_PI);
	sight.SetRange(200);
	
	transform_.position_ = { 110,0,110 };
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
	ModelManager::SetTransform(hModel_, transform_);
	ModelManager::Draw(hModel_);
}

void EnemyBoss::Attack()
{
	if (XMVectorGetX(XMVector3Length(GetToPlayerVec()))<nearRange)
	{
		//UŒ‚1
		//’¼i‚·‚é’e‚ÅUŒ‚

		Shot();
	}
	else if(XMVectorGetX(XMVector3Length(GetToPlayerVec())) < farRange)
	{
		//UŒ‚2
		//’Ç”ö’e‚ÅUŒ‚
		HShot();
	}
}

void EnemyBoss::HShot()
{
	shotTime_++;
	float isShot = 3600.0f / rpm_;
	if (isShot <= (float)shotTime_)
	{
		XMVECTOR shotDir = XMVector3Normalize(GetToPlayerVec());
		HomingBullet* pHBullet = Instantiate<HomingBullet>(this);
		shotTime_ = 0;
	}
}

void EnemyBoss::Shot()
{
	shotTime_++;
	float isShot = 3600.0f / rpm_;
	if (isShot <= (float)shotTime_)
	{
		XMVECTOR shotDir = XMVector3Normalize(GetToPlayerVec());
		Bullet* pBullet = Instantiate<Bullet>(this);
		pBullet->SetDir(shotDir);
		shotTime_ = 0;
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
				ModelManager::DeleteModelNum(hModel_);
				SetIsList(false);
				KillMe();
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
