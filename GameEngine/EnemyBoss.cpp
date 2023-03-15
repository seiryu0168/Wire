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
	//タグ設定
	SetTag("Enemy");
	//プレイヤーのポインター取得
	SetPlayerPointer((Player*)FindObject("Player"));
	//当たり判定設定
	BoxCollider* pCollider = new BoxCollider({0,0,0}, XMFLOAT3(5, 10, 5));
	AddCollider(pCollider);
	
	//コアのモデル読み込み
	hModelCore_ = ModelManager::Load("Assets\\EnemyBossCore.fbx");
	ModelManager::SetModelNum(hModelCore_);
	assert(hModelCore_ >= 0);
	//シールドのモデル読み込み
	hModelShield_ = ModelManager::Load("Assets\\EnemyBossShield.fbx");
	SethModel(hModelShield_);
	ModelManager::SetModelNum(hModelShield_);
	assert(hModelShield_ >= 0);
	
	//視覚の設定
	sight.SetAngle((float)(M_PI*1.5f));
	sight.SetRange(200);
	
	//HP設定
	SetLife(5);
	//位置設定
	transform_.position_ = { 110,10,110 };
	
	//捜索ステートに設定
	ChangeState(StateSearch::GetInstance());
}

void EnemyBoss::Update()
{
	//座標設定
	SetPositionVec(XMLoadFloat3(&transform_.position_));
	pState_->Update(*this);
}

void EnemyBoss::FixedUpdate()
{
}

void EnemyBoss::Draw()
{
	//座標設定
	ModelManager::SetTransform(hModelCore_, transform_);
	//ロックオンされてるなら
	if (IsLockOned(this))
		ModelManager::DrawOutLine(hModelCore_, { 1,0,0,1 });
	//PlayerがAimモードなら
	else if (GetPlayerPointer()->IsAim())
		ModelManager::DrawOutLine(hModelCore_, { 1,1,0,1 });
	//それ以外
	else
		ModelManager::Draw(hModelCore_);
	//HPが3以上なら
	if (GetLife() >= 3)
	{
		//上と同じ
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
	//プレイヤーとの距離が近かったら
	if (XMVectorGetX(XMVector3Length(GetToPlayerVec()))< RANGE_NEAR)
	{
		//攻撃1
		//直進する弾で攻撃
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
	//プレイヤーとの距離が遠かったら
	else if(XMVectorGetX(XMVector3Length(GetToPlayerVec())) < RANGE_FAR)
	{
		//攻撃2
		//追尾弾で攻撃
		HShot(true);
	}
}

void EnemyBoss::HShot(bool shot)
{
	shotTime_++;
	if (shot)
	{
		float isShot = 3600.0f / rpm_;
		//isShotがshotTime_以下だったら
		if (isShot <= (float)shotTime_)
		{
			//HomingBullet生成
			HomingBullet* pHBullet = Instantiate<HomingBullet>(this);
			
			//shotTime_を0にする
			shotTime_ = 0;
			//撃った回数
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
		//isShotがshotTime_以下だったら
		if (isShot <= (float)shotTime_)
		{
			//射撃方向をプレイヤーの方向にする
			XMVECTOR shotDir = XMVector3Normalize(GetToPlayerVec());
			//Bullet生成
			Bullet* pBullet = Instantiate<Bullet>(this);
			//射撃方向設定
			pBullet->SetDir(shotDir);
			//shotTime_を0にする
			shotTime_ = 0;
			//撃った回数
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