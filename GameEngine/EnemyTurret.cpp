#include "EnemyTurret.h"
#include"Engine/ResourceManager/Model.h"
#include"Bullet.h"
#include"HomingBullet.h"
#include"Pointer.h"
#include"Engine/Collider/OBBCollider.h"

namespace
{
	static const int SHOT_COUNT = 10;
}
void EnemyTurret::ChangeSatate(EnemyState<EnemyTurret>* state)
{
	if (state != pState_)
	{
		pState_ = state;
		pState_->Init(*this);
	}
	
}
//コンストラクタ
EnemyTurret::EnemyTurret(GameObject* parent)
	:Enemy(parent,"EnemyTurret"),
	hModel_(-1),
	shotTime_(),
	rpm_(300),
	shotCount_(0),
	reLoadTime_(0),
	RELOAD(90)
{

}

//デストラクタ
EnemyTurret::~EnemyTurret()
{

}

//初期化
void EnemyTurret::Initialize()
{
	SetTag("Enemy");
	OBBCollider* pCollider = new OBBCollider(XMFLOAT3(1, 1, 1), false, false);
	AddCollider(pCollider);
	hModel_ = ModelManager::Load("Assets\\Enemy2.fbx");
	assert(hModel_ >= 0);
	ModelManager::SetModelNum(hModel_);
	
	XMFLOAT3 initPos = transform_.position_;
	initPos.x = (float)(rand() % 100);
	initPos.z = (float)(rand() % 100);
	RayCastData ray;
	ray.start = { initPos.x, 999.0f, initPos.z };
	ray.dir = { 0,1,0 };
	ModelManager::RayCast(hModel_, ray);
	if (ray.hit)
	{
		initPos.y = 999 - ray.dist;
	}
	transform_.position_ = initPos;
	SetPlayerPointer((Player*)FindObject("Player"));
	ChangeSatate(StateSearch::GetInstance());
}

//更新
void EnemyTurret::Update()
{
	SetPositionVec(XMLoadFloat3(&transform_.position_));
	pState_->Update(*this);
}

void EnemyTurret::Attack()
{
}

void EnemyTurret::Shot(bool isShot)
{
		shotTime_++;
	//射撃が許可されてるか
	if (isShot)
	{

		//shotTimeがrpm_を使って計算した値以上になったら
		if ((3600.0f / (float)rpm_) <= (float)shotTime_)
		{
			XMVECTOR shotDir = XMVector3Normalize(GetToPlayerVec());
			Bullet* pBullet = Instantiate<Bullet>(this);
			pBullet->SetDir(shotDir);
			shotCount_++;
			shotTime_ = 0;
		}
	}

}

void EnemyTurret::FixedUpdate()
{

}
//描画
void EnemyTurret::Draw()
{
	ModelManager::SetTransform(hModel_, transform_);
	ModelManager::Draw(hModel_);
}

void EnemyTurret::Release()
{

}

void EnemyTurret::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetTag() == "Player")
	{
		if (GetPlayerPointer()->GetSatatus() & ATC_ATTACK)
		{
			DecreaseLife(1);
			GetPlayerPointer()->SetStatus(ATC_DEFAULT);
			if (GetLife() <= 0)
			{
				Transform pos;
				pos.position_ = { 9999,9999,9999 };
				ModelManager::SetTransform(hModel_, pos);
				SetIsList(false);
				KillMe();
			}
		}
	}
}

void EnemyTurret::StateChase::Init(EnemyTurret& enemy)
{
	enemy.shotTime_ = 0;
	enemy.reLoadTime_ = 0;
}

void EnemyTurret::StateChase::Update(EnemyTurret& enemy)
{
	if (enemy.shotCount_ % 10 == 0 && perShot_)
	{
		enemy.reLoadTime_ = enemy.RELOAD;
		perShot_ = false;
	}
	else
	{
		enemy.reLoadTime_--;
		enemy.reLoadTime_ = max(enemy.reLoadTime_, 0);
	}

	if(enemy.reLoadTime_==0)
	{
		perShot_ = true;
	}
		enemy.Shot(perShot_);


	if (!enemy.IsVisible(enemy.sight.angle_, enemy.sight.range_))
	{
		enemy.ChangeSatate(StateSearch::GetInstance());
	}
}

void EnemyTurret::StateSearch::Init(EnemyTurret& enemy)
{
}

void EnemyTurret::StateSearch::Update(EnemyTurret& enemy)
{
	if (enemy.IsVisible(enemy.sight.angle_, enemy.sight.range_))
	{
		enemy.ChangeSatate(StateChase::GetInstance());
	}
}
